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

#ifndef __GTK_IM_COCOA_VIEW_H__
#define __GTK_IM_COCOA_VIEW_H__

#include <gdk/gdkquartz.h>
#include "gtkimcontextcocoa.h"

@interface GtkIMCocoaView : NSView <NSTextInputClient> {
  GtkIMContextCocoa *gtkIMContextCocoa;
  NSRange markedRange;
  NSRange selectedRange;
  BOOL activated;
}

- (void) setGtkIMContextCocoa: (GtkIMContextCocoa *) context;
- (BOOL) filterKeyDown: (NSEvent *) keyDownEvent;

@end

#endif /* __GTK_IM_COCOA_VIEW_H__ */
