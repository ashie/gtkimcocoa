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

#ifndef __GTK_COM_CONTEXT_COCOA_H__
#define __GTK_COM_CONTEXT_COCOA_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

extern GType gtk_type_im_context_cocoa;

#define GTK_TYPE_IM_CONTEXT_COCOA            gtk_type_im_context_cocoa
#define GTK_IM_CONTEXT_COCOA(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_IM_CONTEXT_COCOA, GtkIMContextCocoa))
#define GTK_IM_CONTEXT_COCOA_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_IM_CONTEXT_COCOA, GtkIMContextCocoaClass))
#define GTK_IS_IM_CONTEXT_COCOA(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_IM_CONTEXT_COCOA))
#define GTK_IS_IM_CONTEXT_COCOA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_IM_CONTEXT_COCOA))
#define GTK_IM_CONTEXT_COCOA_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_IM_CONTEXT_COCOA, GtkIMContextCocoaClass))

typedef struct _GtkIMContextCocoa GtkIMContextCocoa;
typedef struct _GtkIMContextCocoaPrivate GtkIMContextCocoaPrivate;
typedef struct _GtkIMContextCocoaClass GtkIMContextCocoaClass;

struct _GtkIMContextCocoa
{
  GtkIMContext object;
};

struct _GtkIMContextCocoaClass
{
  GtkIMContextClass parent_class;
};


void          gtk_im_context_cocoa_register_type (GTypeModule * type_module);
GtkIMContext *gtk_im_context_cocoa_new           (void);
void          gtk_im_context_cocoa_set_preedit_string
                                                 (GtkIMContextCocoa *context,
                                                  const gchar       *str);
const gchar  *gtk_im_context_cocoa_get_preedit_string
                                                 (GtkIMContextCocoa *context);

G_END_DECLS

#endif /* __GTK_COM_CONTEXT_COCOA_H__ */
