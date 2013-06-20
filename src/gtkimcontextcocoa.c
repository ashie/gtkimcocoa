/*
 * gtkimcocoa
 * Copyright (C) 2013 Takuro Ashie <ashie@clear-code.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "gtkimcontextcocoa.h"
#include <gdk/gdkquartz.h>
#include "GtkCocoaIMClient.h"

#define GTK_IM_COCOA_ENABLE_JIS_KEYBOARD_WORKAROUND 1
#define GTK_IM_COCOA_JIS_HIRAGANA 0x66
#define GTK_IM_COCOA_JIS_EISU     0x68

typedef struct _GtkIMContextCocoaPriv GtkIMContextCocoaPriv;
struct _GtkIMContextCocoaPriv
{
  GdkWindow *client_window;
  gchar *preedit_string;
  GdkRectangle cursor_location;
  gint cursor_pos;
  gint selected_len;
  gboolean focused;
};

GtkCocoaIMClient *im_client = NULL;
GStaticMutex im_client_mutex = G_STATIC_MUTEX_INIT;

#define GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), GTK_TYPE_IM_CONTEXT_COCOA, GtkIMContextCocoaPriv))

/* GObject class functions */
static void gtk_im_context_cocoa_class_init (GtkIMContextCocoaClass *class);
static void gtk_im_context_cocoa_init       (GtkIMContextCocoa      *context_cocoa);
static void dispose    (GObject *obj);
static void finalize   (GObject *obj);

static void set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec);
static void get_property (GObject      *object,
                          guint         prop_id,
                          GValue       *value,
                          GParamSpec   *pspec);

/* GtkIMContext class functions */
static void set_client_window   (GtkIMContext *context,
                                 GdkWindow    *client_window);
static gboolean filter_keypress (GtkIMContext   *context,
                                 GdkEventKey    *event);
static void reset               (GtkIMContext   *context);
static void get_preedit_string  (GtkIMContext   *context,
                                 gchar         **str,
                                 PangoAttrList **attrs,
                                 gint           *cursor_pos);
static void focus_in            (GtkIMContext   *context);
static void focus_out           (GtkIMContext   *context);
static void set_cursor_location (GtkIMContext   *context,
                                 GdkRectangle   *area);
static void set_use_preedit     (GtkIMContext   *context,
                                 gboolean        use_preedit);

GType gtk_type_im_context_cocoa = 0;
static GObjectClass *parent_class;

void
gtk_im_context_cocoa_register_type (GTypeModule *type_module)
{
  const GTypeInfo im_context_cocoa_info = {
    sizeof (GtkIMContextCocoaClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) gtk_im_context_cocoa_class_init,
    NULL, /* class_finalize */
    NULL, /* class_data */
    sizeof (GtkIMContextCocoa),
    0,
    (GInstanceInitFunc) gtk_im_context_cocoa_init,
  };

  gtk_type_im_context_cocoa =
    g_type_module_register_type (type_module,
                                 GTK_TYPE_IM_CONTEXT,
                                 "GtkIMContextCocoa", &im_context_cocoa_info, 0);
}

static void
gtk_im_context_cocoa_class_init (GtkIMContextCocoaClass *class)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS (class);
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  parent_class = g_type_class_peek_parent (class);

  gobject_class->finalize     = finalize;
  gobject_class->dispose      = dispose;
  gobject_class->set_property = set_property;
  gobject_class->get_property = get_property;

  im_context_class->set_client_window   = set_client_window;
  im_context_class->filter_keypress     = filter_keypress;
  im_context_class->reset               = reset;
  im_context_class->get_preedit_string  = get_preedit_string;
  im_context_class->focus_in            = focus_in;
  im_context_class->focus_out           = focus_out;
  im_context_class->set_cursor_location = set_cursor_location;
  im_context_class->set_use_preedit     = set_use_preedit;

  g_type_class_add_private(gobject_class, sizeof(GtkIMContextCocoaPriv));
}

static void
set_focused_context (GtkIMContextCocoa *context_cocoa)
{
  g_static_mutex_lock(&im_client_mutex);

  if (!im_client) {
    NSRect rect = NSMakeRect(0, 0, 0, 0);
    im_client = [[GtkCocoaIMClient alloc] initWithFrame:rect];
  }
  [im_client setGtkIMContextCocoa: context_cocoa];

  g_static_mutex_unlock(&im_client_mutex);
}

static void
gtk_im_context_cocoa_init (GtkIMContextCocoa *context_cocoa)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context_cocoa);

  priv->client_window = NULL;
  priv->preedit_string = g_strdup("");
  priv->cursor_pos = 0;
  priv->selected_len = 0;
  priv->focused = FALSE;
}

static void
dispose (GObject *obj)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(obj);

  if (priv->preedit_string) {
    g_free(priv->preedit_string);
    priv->preedit_string = NULL;
  }

  if (G_OBJECT_CLASS (parent_class)->dispose)
    G_OBJECT_CLASS (parent_class)->dispose (obj);
}

static void
finalize (GObject *obj)
{
  if (G_OBJECT_CLASS (parent_class)->finalize)
    G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
}

static void
get_property (GObject    *object,
              guint       prop_id,
              GValue     *value,
              GParamSpec *pspec)
{
}

GtkIMContext *
gtk_im_context_cocoa_new (void)
{
  return g_object_new (GTK_TYPE_IM_CONTEXT_COCOA, NULL);
}

static void
set_client_window (GtkIMContext *context,
                   GdkWindow    *client_window)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  priv->client_window = client_window;
}

static gboolean
filter_keypress (GtkIMContext *context,
                 GdkEventKey  *event)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);
  gboolean handled = FALSE;

  if (!priv->focused)
    return handled;

  if (event->type == GDK_KEY_PRESS) {
    NSEvent *nsevent = gdk_quartz_event_get_nsevent((GdkEvent*)event);

    g_static_mutex_lock(&im_client_mutex);
    handled = [im_client filterKeyDown: nsevent];
    g_static_mutex_unlock(&im_client_mutex);
  }

#ifdef GTK_IM_COCOA_ENABLE_JIS_KEYBOARD_WORKAROUND
  /*
   * GdkQuartz (gtk+-3.8.2) translates these keys to GDK_space unexpectedly.
   * Although we should fix it at GdkQuartz layer, we also handle these keys
   * at this layer to work with old version of GTK+.
   */
  if (event->hardware_keycode == GTK_IM_COCOA_JIS_HIRAGANA ||
      event->hardware_keycode == GTK_IM_COCOA_JIS_EISU)
  {
    handled = TRUE;
  }
#endif

  return handled;
}

static void
reset (GtkIMContext *context)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  g_free(priv->preedit_string);
  priv->preedit_string = g_strdup("");
  priv->cursor_pos = 0;
  priv->selected_len = 0;
}

static PangoAttrList *
create_pango_attr_list (GtkIMContextCocoaPriv *priv)
{
  PangoAttrList *attrs;
  PangoAttribute *attr;
  gint text_len = g_utf8_strlen(priv->preedit_string, -1);
  gchar *head = priv->preedit_string;
  gint last_pos = 0;
  gint selected_tail_pos = priv->cursor_pos + priv->selected_len;

#define ADD_ATTR(tail_pos, type) \
{ \
  gchar *tail = g_utf8_offset_to_pointer(priv->preedit_string, tail_pos); \
  attr = pango_attr_underline_new(type); \
  attr->start_index = last_pos; \
  attr->end_index = attr->start_index + (tail - head); \
  pango_attr_list_change(attrs, attr); \
  head = tail; \
  last_pos = attr->end_index; \
}

  attrs = pango_attr_list_new();

  if (!priv->preedit_string || !*priv->preedit_string)
    return attrs;

  if (priv->cursor_pos > 0)
    ADD_ATTR(priv->cursor_pos, PANGO_UNDERLINE_SINGLE);
  if (priv->selected_len > 0)
    ADD_ATTR(priv->cursor_pos + priv->selected_len, PANGO_UNDERLINE_DOUBLE);
  if (head != priv->preedit_string && selected_tail_pos)
    ADD_ATTR(text_len, PANGO_UNDERLINE_SINGLE);

#undef ADD_ATTR

  return attrs;
}

static void
get_preedit_string (GtkIMContext   *context,
                    gchar         **str,
                    PangoAttrList **attrs,
                    gint           *cursor_pos)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  if (str)
    *str = g_strdup(priv->preedit_string);
  if (attrs)
    *attrs = create_pango_attr_list(priv);
  if (cursor_pos)
    *cursor_pos = priv->cursor_pos;
}

static void
focus_in (GtkIMContext *context)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);
  set_focused_context(GTK_IM_CONTEXT_COCOA(context));
  priv->focused = TRUE;
}

static void
focus_out (GtkIMContext *context)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);
  priv->focused = FALSE;
  set_focused_context(NULL);
}

static void
set_cursor_location (GtkIMContext *context,
                     GdkRectangle *area)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);
  if (area)
    priv->cursor_location = *area;
}

static void
set_use_preedit (GtkIMContext *context,
                 gboolean      use_preedit)
{
}

void
gtk_im_context_cocoa_set_preedit_string (GtkIMContextCocoa *context,
                                         const gchar       *str,
                                         gint               cursor_pos,
                                         gint               selected_len)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  g_free(priv->preedit_string);
  priv->preedit_string = g_strdup(str ? str : "");
  priv->cursor_pos = cursor_pos;
  priv->selected_len = selected_len;

  g_signal_emit_by_name(context, "preedit-changed");
}

const gchar *
gtk_im_context_cocoa_get_preedit_string (GtkIMContextCocoa *context)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  return priv->preedit_string;
}

GdkRectangle
gtk_im_context_cocoa_get_cursor_location (GtkIMContextCocoa *context)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);
  GdkRectangle location = {0, 0, 0, 0};
  /* FIXME: should be detected */
  gint string_height = 24;

  if (priv->client_window) {
    location = priv->cursor_location;
    location.height = string_height;
    gdk_window_get_root_coords(priv->client_window,
                               location.x, location.y,
                               &location.x, &location.y);
  }

  return location;
}
