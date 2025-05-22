/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef RUN_MODEL_H__
#define RUN_MODEL_H__

#include <gtk/gtk.h>
#include "pneumatic_leak_test.h"
G_BEGIN_DECLS

#define RUN_TYPE_MODEL              ( run_model_get_type( ) )

G_DECLARE_FINAL_TYPE(RunModel, run_model, RUN, MODEL, GObject)

#define RUN_MODEL_MODE_CHANGE_PROP_STR          "mode-changed"
#define RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR    "step-changed"
#define RUN_MODEL_SETUP_LEAK_COMPLETE_PROP_STR    "leak-check-complete"
#define RUN_MODEL_PNEUMATIC_LEAK_TEST_PROP_STR     "pneumatic-leak-test"


#define RUN_MODEL_MODE_CHANGE_SIGNAL_STR        "run-mode-changed"
#define RUN_MODEL_SETUP_STEP_CHANGE_SIGNAL_STR  "setup-step-changed"

typedef enum
{
   RUN_MODE_NOT_SET = 0,
   RUN_MODE_RESTORE,
   RUN_MODE_STANDARD,
   RUN_MODE_TEST,
   N_RUM_MODEL_MODES
} RUN_MODEL_MODE;



typedef enum
{
   RUN_SETUP_UNINITIALIZED = 0,
   RUN_SETUP_MODE_SELECTED,

   RUN_SETUP_START_PNEUMATICS_TEST,
   RUN_SETUP_WAIT_FOR_PNEUMATICS_TEST,
   RUN_SETUP_PROCESS_PNEUMATICS_TEST_RESULT,
   RUN_SETUP_PNEUMATICS_TEST_COMPLETE_FAILURE,

   RUN_SETUP_MEMCHECK_COMPLETE,
   // ...
   RUN_SETUP_INTERMEDIATE_STEPS,
   // ...
   RUN_SETUP_PROMPT_FOR_FLUID_VOLUME,
   // ...
   RUN_SETUP_COMPLETE,
   RUN_SETUP_FAILED,
   RUN_SETUP_N_STEPS
} RUN_SETUP_STEPS;

typedef void (*RunModelSetterCallback_T)(RunModel *self, gpointer user_data);
typedef gboolean (* RunModelBooleanGetterFn_T) (RunModel *self);

typedef gboolean (*PromptStrValidateAndSetFn)(RunModel *self, const gchar *input_str);


/* Methods for our newly declared GObject derived GType*/
RunModel *run_model_new();

RUN_MODEL_MODE run_model_get_run_mode(RunModel *self );
void run_model_set_run_mode(RunModel *self, RUN_MODEL_MODE mode );

RUN_SETUP_STEPS run_model_get_step(RunModel *self );
void run_model_set_step(RunModel *self, RUN_SETUP_STEPS step );

gchar *run_model_get_run_description(RunModel *self);
void run_model_set_run_description(RunModel *self, const gchar *run_description);

gboolean run_model_get_leak_check_complete(RunModel *self);
void run_model_set_leak_check_complete(RunModel *self, gboolean complete);

gboolean run_model_get_leak_check_success(RunModel *self);
PneumaticLeakTest *run_model_get_pneumatic_leak_test(RunModel *self);
void run_model_set_pneumatic_leak_test(RunModel *self, PneumaticLeakTest *source);

guint run_model_get_fluid_volume(RunModel *self);
gboolean run_model_validate_and_set_fluid_volume(RunModel *self, const gchar *ui_input_str);

// Memento?
void run_model_set_restore_point(RunModel *self);
RunModel *run_model_get_restore_point(RunModel *self);

G_END_DECLS
#endif  // RUN_MODEL_H__
