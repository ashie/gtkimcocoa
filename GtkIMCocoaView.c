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

#include "GtkIMCocoaView.h"

@implementation GtkIMCocoaView

- (id) init
{
    [super init];
    return self;
}

- (void) setGtkIMContextCocoa: (GtkIMContextCocoa *) context
{
    gtkIMContextCocoa = context;
}

- (BOOL) filterKeyDown: (NSEvent *) keyDownEvent
{
    g_message("filterKeyDown");

    /*
     * Since a event whose type is "NSFlagsChanged" throw exception, we ignore it.
     */
    if ([keyDownEvent type] == NSFlagsChanged)
        return NO;

    /*
     * FIXME:
     * Although the reference manual says "shouldn't call directly",
     * I can't get preedit events without it.
     * http://developer.apple.com/library/mac/documentation/cocoa/reference/NSTextInputContext_Class/Reference/Reference.html#//apple_ref/occ/instm/NSTextInputContext/activate
     */
    [[self inputContext] activate];

    return [[self inputContext] handleEvent:keyDownEvent];
}

- (void) keyDown: (NSEvent *)theEvent
{
    [self filterKeyDown:theEvent];
}

- (NSUInteger) characterIndexForPoint: (NSPoint)aPoint
{
    g_message("characterIndexForPoint");
    return 0;
}

- (NSRect) firstRectForCharacterRange:
    (NSRange)aRange
    actualRange:(NSRangePointer)actualRange
{
    GdkRectangle location;
    NSRect rect;

    location = gtk_im_context_cocoa_get_cursor_location(gtkIMContextCocoa);
    location.y = gdk_screen_height() - location.y - location.height;
    rect = NSMakeRect(location.x, location.y,
                      location.width, location.height);

    g_message("firstRectForCharacterRange");

    return rect;
}

- (NSArray *) validAttributesForMarkedText
{
    g_message("validAttributesForMarkedText");
    return NULL;
}

- (NSAttributedString *) attributedSubstringForProposedRange:
    (NSRange)aRange
    actualRange:(NSRangePointer)actualRange
{
    g_message("attributedSubstringForProposedRange");
    return NULL;
}

- (BOOL) hasMarkedText
{
    const gchar *text = gtk_im_context_cocoa_get_preedit_string(gtkIMContextCocoa);
    g_message("hasMarkedText");
    if (text && *text)
        return YES;
    else
        return NO;
}

- (NSRange) markedRange
{
    const gchar *text = gtk_im_context_cocoa_get_preedit_string(gtkIMContextCocoa);

    g_message("markedRange");

    if (text && *text)
        return NSMakeRange(0, g_utf8_strlen(text, -1));
    else
        return NSMakeRange(NSNotFound, 0);
}

- (NSRange) selectedRange
{
    g_message("selectedRange");
    return selectedRange;
}

- (void) unmarkText
{
    gtk_im_context_cocoa_set_preedit_string(gtkIMContextCocoa, "", 0);
    g_message("unmarkText");
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
    gtk_im_context_cocoa_set_preedit_string(gtkIMContextCocoa, str,
                                            selectedRange.location);

    g_message("setMarkedText: %s", str);
}

- (void) doCommandBySelector: (SEL)aSelector
{
    g_message("doCommandBySelector");
    [super doCommandBySelector:aSelector];
}

- (void) insertText: (id)aString replacementRange:(NSRange)replacementRange
{
    const char *str;
    
    if ([aString isKindOfClass:[NSAttributedString class]]) {
        str = [[aString string] UTF8String];
    } else {
        str = [aString UTF8String];
    }

    gtk_im_context_cocoa_set_preedit_string(gtkIMContextCocoa, "", 0);
    g_signal_emit_by_name(gtkIMContextCocoa, "commit", str);

    g_message("insertText: \"%s\"", str);
}


@end
