#ifndef PNEUMATIC_LEAK_TEST_H__
#define PNEUMATIC_LEAK_TEST_H__
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define PNEUMATIC_TYPE_LEAK_TEST (pneumatic_leak_test_get_type())

typedef struct PneumaticLeakTest
{
   gboolean    test_started;
   gboolean    test_completed;
   gboolean    test_passed;
} PneumaticLeakTest;

GType pneumatic_leak_test_get_type(void) G_GNUC_CONST;

PneumaticLeakTest *pneumatic_leak_test_new();

void pneumatic_leak_test_reset(PneumaticLeakTest *self);

PneumaticLeakTest *pneumatic_leak_test_copy(PneumaticLeakTest *source);

gboolean pneumatic_leak_test_equal(PneumaticLeakTest *left, PneumaticLeakTest *right);

void pneumatic_leak_test_free(PneumaticLeakTest *self);

G_END_DECLS
#endif  // PNEUMATIC_LEAK_TEST_H__
