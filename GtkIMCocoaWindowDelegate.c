#include "GtkIMCocoaWindowDelegate.h"
#include <glib.h>

@implementation GtkIMCocoaWindowDelegate
- (id)windowWillReturnFieldEditor:(NSWindow *)sender toObject:(id) cleint {
  g_print("windowWillReturnFieldEditor\n");
  return nil;
}
@end
