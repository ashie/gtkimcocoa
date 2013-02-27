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

- (id) init {
    [super init];
    markedText = NULL;
    return self;
}

- (void) setGtkIMContextCocoa: (GtkIMContextCocoa *) context {
    gtkIMContextCocoa = context;
}

- (void) keyDown:(NSEvent *)theEvent {
    g_message("keyDown");

    /*
     * FIXME:
     * Although the reference manual says "shouldn't call directly",
     * I can't get preedit events without it.
     * http://developer.apple.com/library/mac/documentation/cocoa/reference/NSTextInputContext_Class/Reference/Reference.html#//apple_ref/occ/instm/NSTextInputContext/activate
     */
    [[self inputContext] activate];

    [[self inputContext] handleEvent:theEvent];
}

- (NSUInteger)characterIndexForPoint:(NSPoint)aPoint {
    g_message("characterIndexForPoint");
    return 0;
}

- (NSRect)firstRectForCharacterRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
    g_message("firstRectForCharacterRange");
    return NSMakeRect(0, 0, 0, 0);
}

- (NSArray *)validAttributesForMarkedText {
    g_message("validAttributesForMarkedText");
    return NULL;
}

- (NSAttributedString *)attributedSubstringForProposedRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
    g_message("attributedSubstringForProposedRange");
    return NULL;
}

- (BOOL)hasMarkedText {
    g_message("hasMarkedText");
    if (markedText && *markedText)
        return YES;
    else
        return NO;
}

- (NSRange)markedRange {
    g_message("markedRange");
    return NSMakeRange(0, 0);
}

- (NSRange)selectedRange {
    g_message("selectedRange");
    return NSMakeRange(0, 0);
}

- (void)unmarkText {
    g_message("unmarkText");
}

- (void)setMarkedText:(id)aString selectedRange:(NSRange)newSelection replacementRange:(NSRange)replacementRange {
    const char *str;

    if ([aString isKindOfClass:[NSAttributedString class]]) {
        str = [[aString string] UTF8String];
    } else {
        str = [aString UTF8String];
    }

    g_free(markedText);
    markedText = g_strdup(str);
    gtk_im_context_cocoa_set_preedit_string(gtkIMContextCocoa, str);

    g_message("setMarkedText: %s", str);
}

- (void)doCommandBySelector:(SEL)aSelector {
    g_message("doCommandBySelector");
    [super doCommandBySelector:aSelector];
}

- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange {
    const char *str;
    
    if ([aString isKindOfClass:[NSAttributedString class]]) {
        str = [[aString string] UTF8String];
    } else {
        str = [aString UTF8String];
    }

    g_signal_emit_by_name(gtkIMContextCocoa, "commit", str);

    g_message("insertText: \"%s\"", str);
}


@end
