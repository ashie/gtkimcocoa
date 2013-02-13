#include "GtkIMCocoaView.h"

@implementation GtkIMCocoaView

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

    g_message("insertText: \"%s\"", str);
}


@end
