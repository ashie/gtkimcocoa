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
#include "GtkIMCocoaView.h"

typedef struct _GtkIMContextCocoaPriv GtkIMContextCocoaPriv;
struct _GtkIMContextCocoaPriv
{
  GdkWindow *client_window;
  GtkIMCocoaView *view;
  gchar *preedit_string;
  gint cursor_pos;
};

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
gtk_im_context_cocoa_init (GtkIMContextCocoa *context_cocoa)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context_cocoa);
  NSRect rect = NSMakeRect(0, 0, 0, 0);

  priv->client_window = NULL;
  priv->view = [[GtkIMCocoaView alloc] initWithFrame:rect];
  [priv->view setGtkIMContextCocoa: context_cocoa];
  priv->preedit_string = g_strdup("");
  priv->cursor_pos = 0;
}

static void
dispose (GObject *obj)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(obj);

  if (priv->preedit_string) {
    g_free(priv->preedit_string);
    priv->preedit_string = NULL;
  }

  if (priv->view) {
    [priv->view setGtkIMContextCocoa: nil];
    [priv->view release];
    priv->view = NULL;
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
  if (event->type == GDK_KEY_PRESS) {
    NSEvent *nsevent = gdk_quartz_event_get_nsevent((GdkEvent*)event);
    [priv->view keyDown: nsevent];
  }
  return FALSE;
}

static void
reset (GtkIMContext *context)
{
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
    *attrs = pango_attr_list_new();
  if (cursor_pos)
    *cursor_pos = priv->cursor_pos;
}

static void
focus_in (GtkIMContext *context)
{
}

static void
focus_out (GtkIMContext *context)
{
}

static void
set_cursor_location (GtkIMContext *context,
                     GdkRectangle *area)
{
}

static void
set_use_preedit (GtkIMContext *context,
                 gboolean      use_preedit)
{
}

void
gtk_im_context_cocoa_set_preedit_string (GtkIMContextCocoa *context,
                                         const gchar       *str,
                                         gint               cursor_pos)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  g_free(priv->preedit_string);
  priv->preedit_string = g_strdup(str ? str : "");
  priv->cursor_pos = cursor_pos;

  g_signal_emit_by_name(context, "preedit-changed");
}

const gchar *
gtk_im_context_cocoa_get_preedit_string (GtkIMContextCocoa *context)
{
  GtkIMContextCocoaPriv *priv = GET_PRIVATE(context);

  return priv->preedit_string;
}
