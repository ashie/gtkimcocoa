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

#include "GtkCocoaIMClient.h"

@implementation GtkCocoaIMClient

- (id) initWithFrame: (NSRect) frame
{
  [super initWithFrame: frame];

  markedRange = NSMakeRange(NSNotFound, 0);
  selectedRange = NSMakeRange(NSNotFound, 0);
  activated = NO;
  handled = NO;
  return self;
}

- (void) setGtkIMContextCocoa: (GtkIMContextCocoa *) context
{
  gtkIMContextCocoa = context;

  /*
   * Although Apple's reference manual says "You should not call this method
   * directly", we should do it because a NSWindow is needed to activate the
   * inputContext automatically. This NSView never have NSWindow.
   */
  if (!activated) {
    [[self inputContext] activate];
    activated = YES;
  }
}

- (BOOL) filterKeyDown: (NSEvent *) keyDownEvent
{
  /*
   * Since an event whose type is "NSFlagsChanged" throws exception,
   * we ignore it.
   */
  if ([keyDownEvent type] == NSFlagsChanged)
    return NO;

  handled = NO;
  [self interpretKeyEvents:[NSArray arrayWithObject:keyDownEvent]];
  return handled;
}

- (void) keyDown: (NSEvent *)theEvent
{
  [self filterKeyDown:theEvent];
}

- (NSUInteger) characterIndexForPoint: (NSPoint)aPoint
{
  return 0;
}

/*
 * Because _gdk_quartz_window_gdk_xy_to_xy() cannot be used from external
 * program, we do same calculation here.
 */
static void
gdk_rect_to_ns_rect(GdkRectangle *rect)
{
  NSArray *screens = [NSScreen screens];
  gint min_x = 0, min_y = 0, max_x = 0, max_y = 0, height, i;

  for (i = 0; i < [screens count]; i++) {
    NSRect screen_rect = [[screens objectAtIndex:i] frame];
    min_x = MIN (min_x, screen_rect.origin.x);
    max_x = MAX (max_x, screen_rect.origin.x);
    min_y = MIN (min_y, screen_rect.origin.y);
    max_y = MAX (max_y, screen_rect.origin.y + screen_rect.size.height);
  }

  height = max_y - min_y;

  rect->x = rect->x + min_x;
  rect->y = height - (rect->y + rect->height) + min_y;
}

- (NSRect) firstRectForCharacterRange:
  (NSRange)aRange
  actualRange:(NSRangePointer)actualRange
{
  GdkRectangle location;

  location = gtk_im_context_cocoa_get_cursor_location(gtkIMContextCocoa);
  gdk_rect_to_ns_rect(&location);

  return NSMakeRect(location.x, location.y,
                    location.width, location.height);
}

- (NSArray *) validAttributesForMarkedText
{
  return nil;
}

- (NSAttributedString *) attributedSubstringForProposedRange:
  (NSRange)aRange
  actualRange:(NSRangePointer)actualRange
{
  return nil;
}

- (BOOL) hasMarkedText
{
  const gchar *text = gtk_im_context_cocoa_get_preedit_string(gtkIMContextCocoa);

  if (text && *text)
    return YES;
  else
    return NO;
}

- (NSRange) markedRange
{
  return markedRange;
}

- (NSRange) selectedRange
{
  return selectedRange;
}

- (void) unmarkText
{
  handled = YES;
  gtk_im_context_cocoa_set_preedit_string(gtkIMContextCocoa, "", 0, 0);
}

- (void) setMarkedText: (id)aString
  selectedRange:(NSRange)newSelection
  replacementRange:(NSRange)replacementRange
{
  const char *str;

  if ([aString isKindOfClass:[NSAttributedString class]]) {
    str = [[aString string] UTF8String];
  } else {
    str = [aString UTF8String];
  }

  selectedRange = newSelection;
  if (str && *str)
    markedRange = NSMakeRange(0, g_utf8_strlen(str, -1));
  else
    markedRange = NSMakeRange(NSNotFound, 0);

  handled = YES;
  gtk_im_context_cocoa_set_preedit_string(gtkIMContextCocoa, str,
                                          selectedRange.location,
                                          selectedRange.length);
}

- (void) doCommandBySelector: (SEL)aSelector
{
  /*
   * Do nothing. Although we are required to implement this method, we do not
   * handle Cocoa command IDs, and the Apple docs explcitly say not to forward
   * unhandled commands up the NSReponder chain, which calls NSBeep().
   */
}

- (void) insertText: (id)aString replacementRange:(NSRange)replacementRange
{
  const char *str;
    
  if ([aString isKindOfClass:[NSAttributedString class]]) {
    str = [[aString string] UTF8String];
  } else {
    str = [aString UTF8String];
  }

  [self unmarkText];

  handled = YES;
  g_signal_emit_by_name(gtkIMContextCocoa, "commit", str);
}


@end
