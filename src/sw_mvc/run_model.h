/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef RUN_MODEL_H__
#define RUN_MODEL_H__

#include <gtk/gtk.h>
G_BEGIN_DECLS

#define RUN_TYPE_MODEL              ( run_model_get_type( ) )

G_DECLARE_FINAL_TYPE(RunModel, run_model, RUN, MODEL, GObject)

#define RUN_MODEL_MODE_CHANGE_PROP_STR          "mode-changed"
#define RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR    "step-changed"
#define RUN_MODEL_SETUP_LEAK_COMPLETE_PROP_STR    "leak-check-complete"
#define RUN_MODEL_PNEUMATIC_LEAK_TEST_PROP_STR     "pneumatic-leak-test"

#define RM_PUMPING_PROP_STR                     "pumping"

#define RM_QR_CODE_COMPLETE_PROP_STR            "qr-code-complete"

#define RM_PERF_PRESS_CAL_COMPLETE_PROP_STR   "perf-pres-cal-complete"
#define RM_PERF_FLOW_MEAS_COMPLETE_PROP_STR   "perf-flow-meas-complete"
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
   RUN_SETUP_INSTRUCT_PREHEAT_BLOOD,
   RUN_SETUP_INSTRUCT_LOAD_DISPOSABLE,
   RUN_SETUP_START_PNEUMATICS_TEST,
   RUN_SETUP_PNEUMATICS_TEST_COMPLETE_FAILURE,
   RUN_SETUP_INSTRUCT_QR_CODE,
   RUN_SETUP_START_QR_CODE_READING,

   RUN_SETUP_COMMAND_LOAD_ALBUMIN,
   RUN_SETUP_INSTRUCT_PRIME_LUER_PORTS,
   RUN_SETUP_INSTRUCT_PRIME_BUBBLE_TRAP,
   RUN_SETUP_CMDOBSERVER_PERFUSION_PRESS_CAL,
   RUN_SETUP_CMDOBSERVER_PERFUSION_FLOW_MEAS,
   RUN_SETUP_AUTOCMD_START_ALBUMIN_PRIMING,
   RUN_SETUP_INSTRUCT_CHECK_FOR_LEAKS,
   RUN_SETUP_INSTRUCT_ADD_SODIUM_BICARB,
   RUN_SETUP_INSTRUCT_CONNECT_CLIMIMIX,
   RUN_SETUP_COMMAND_PRIME_NUTRITION_TUBING,

   RUN_SETUP_PROMPT_FOR_FLUID_VOLUME,
   RUN_SETUP_AUTOCMD_SWITCH_TO_BLOOD_PARAMS,
   RUN_SETUP_COMMAND_FEED_RBC_START_BLOOD_OXYGENATION,

   RUN_SETUP_INSTRUCT_LOAD_REAGENTS,

   RUN_SETUP_MEMCHECK_START,

   RUN_SETUP_AUTO_RETRY_CMD,
   RUN_SOME_OTHER_IN_BETWEEN_STEP,
   RUN_SETUP_MEMCHECK_COMPLETE,
   // ...
   RUN_SETUP_INTERMEDIATE_STEPS,


   // ...
   RUN_SETUP_COMPLETE,
   RUN_SETUP_FAILED,
   RUN_SETUP_N_STEPS
} RUN_SETUP_STEPS;

typedef void (*RunModelCommandFn_T)(RunModel *model);
typedef void (*RunModelSetterCallback_T)(RunModel *self, gpointer user_data);
typedef gboolean (* RunModelBooleanGetterFn_T) (RunModel *self);

typedef gboolean (*PromptStrValidateAndSetFn)(RunModel *self, const gchar *input_str);


/* Methods for our newly declared GObject derived GType*/
RunModel *run_model_new();

RUN_MODEL_MODE run_model_get_run_mode(RunModel *self );
void run_model_set_run_mode(RunModel *self, RUN_MODEL_MODE mode );

RUN_SETUP_STEPS run_model_get_next_step(RunModel *self );
void run_model_set_next_step(RunModel *self, RUN_SETUP_STEPS step );

gchar *run_model_get_run_description(RunModel *self);
void run_model_set_run_description(RunModel *self, const gchar *run_description);

guint run_model_get_fluid_volume(RunModel *self);
gboolean run_model_validate_and_set_fluid_volume(RunModel *self, const gchar *ui_input_str);

gboolean run_model_get_pumping(RunModel *self);
void run_model_set_pumping(RunModel *self, gboolean pumping);

gboolean run_model_get_leak_check_result(RunModel *self);
void run_model_set_leak_check_result(RunModel *self, gboolean passed);
gboolean run_model_get_leak_check_complete(RunModel *self);
void run_model_set_leak_check_complete(RunModel *self, gboolean complete);

gboolean run_model_get_qr_read_complete(RunModel *self);
void run_model_set_qr_read_complete(RunModel *self, gboolean complete);

gboolean run_model_get_perf_press_cal_result(RunModel *self);
void run_model_set_perf_press_cal_result(RunModel *self, gboolean passed);
gboolean run_model_get_perf_press_cal_complete(RunModel *self);
void run_model_set_perf_press_cal_complete(RunModel *self, gboolean complete);

gboolean run_model_get_perf_flow_meas_result(RunModel *self);
void run_model_set_perf_flow_meas_result(RunModel *self, gboolean passed);
gboolean run_model_get_perf_flow_meas_complete(RunModel *self);
void run_model_set_perf_flow_meas_complete(RunModel *self, gboolean complete);

void run_model_cmd_start_leak_check(RunModel *self);
void run_model_cmd_load_albumin(RunModel *self);
void run_model_cmd_start_pumping(RunModel *self);
void run_model_cmd_stop_pumping(RunModel *self);
void run_model_cmd_start_perf_press_cal_check(RunModel *self);
void run_model_cmd_start_perf_flow_meas_check(RunModel *self);
void run_model_cmd_start_priming_nutrition_tubing(RunModel *self);
void run_model_cmd_set_blood_params(RunModel *self);
void run_model_cmd_start_blood_oxygenation(RunModel *self);

guint run_model_get_blood_volume(RunModel *self);
gboolean run_model_validate_and_set_blood_volume(RunModel *self, const gchar *ui_input_str);


// Memento?
void run_model_set_restore_point(RunModel *self);
RunModel *run_model_get_restore_point(RunModel *self);

G_END_DECLS
#endif  // RUN_MODEL_H__
