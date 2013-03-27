/*
 * gtkimcocoa
 * Copyright (C) 2013 Takuro Ashie
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

#include "config.h"
#include <string.h>

#include "gtk/gtkimmodule.h"
#include "gtkimcontextcocoa.h"

static const GtkIMContextInfo cocoa_info = {
  "cocoa",
  "Mac OS X Cocoa",
  "gtk+",
  "",
  "ja:ko:zh",
};

static const GtkIMContextInfo *info_list[] = {
  &cocoa_info,
};

#ifndef INCLUDE_IM_cocoa
#define MODULE_ENTRY(type,function) G_MODULE_EXPORT type im_module_ ## function
#else
#define MODULE_ENTRY(type, function) type _gtk_immodule_cocoa_ ## function
#endif

MODULE_ENTRY (void, init) (GTypeModule *module)
{
  gtk_im_context_cocoa_register_type (module);
}

MODULE_ENTRY (void, exit) (void)
{
}

MODULE_ENTRY (void, list) (const GtkIMContextInfo ***contexts, int *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS (info_list);
}

MODULE_ENTRY (GtkIMContext *, create) (const gchar *context_id)
{
  g_return_val_if_fail (context_id, NULL);

  if (!strcmp (context_id, "cocoa"))
    return g_object_new (GTK_TYPE_IM_CONTEXT_COCOA, NULL);
  else
    return NULL;
}
