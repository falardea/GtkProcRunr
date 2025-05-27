/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include "run_model.h"
#include "utils/logging.h"

typedef struct
{
   gboolean keep_alive;
}RunModelPrivate;

struct _RunModel
{
   GObject           super;

   gchar    *run_description;

   RUN_MODEL_MODE    run_mode;
   RUN_SETUP_STEPS   last_completed_phase;

   guint    fluid_volume;

   gboolean          pumping;
   gboolean          leak_check_running;
   gboolean          leak_check_complete;
   gboolean          leak_check_success;

   gboolean          perf_press_cal_running;
   gboolean          perf_press_cal_complete;
   gboolean          perf_press_cal_success;
   gboolean          perf_flow_meas_running;
   gboolean          perf_flow_meas_complete;
   gboolean          perf_flow_meas_success;

   gboolean          qr_code_complete;
};

G_DEFINE_TYPE_WITH_PRIVATE( RunModel, run_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   RUN_MODEL_PROP_RUN_MODE,
   RUN_MODEL_PROP_PUMPING,
   RUN_MODEL_PROP_SETUP_STEP_CHANGE,
   RUN_MODEL_PROP_LEAK_CHECK_COMPLETE,
   RUN_MODEL_PROP_QR_CODE_COMPLETE,
   RUN_MODEL_PROP_PERF_PRESS_CAL_COMPLETE,
   RUN_MODEL_PROP_PERF_FLOW_MEAS_COMPLETE,
   RUN_MODEL_N_PROPERTIES
};

enum
{
   RUN_MODEL_SIGNAL_MODE_CHANGE,
   RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE,
   RUN_MODEL_N_SIGNALS
};

void (* run_model_listener_response) (RunModel *model, RUN_MODEL_MODE mode);
void (* run_step_listener_response) (RunModel *model, RUN_SETUP_STEPS step);

static void run_model_finalize( GObject *g_obj )
{
   RunModel *self = RUN_MODEL(g_obj);
   if(self->run_description)
   {
      g_free(self->run_description);
   }
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS (run_model_parent_class)->finalize (g_obj);
}

static void run_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_MODE:
         run_model_set_run_mode( model, g_value_get_uint( value ) );
         break;
      case RUN_MODEL_PROP_SETUP_STEP_CHANGE:
         run_model_set_next_step( model, g_value_get_uint( value ) );
         break;
      case RUN_MODEL_PROP_LEAK_CHECK_COMPLETE:
         run_model_set_leak_check_complete(model, g_value_get_boolean( value ) );
         break;
      case RUN_MODEL_PROP_QR_CODE_COMPLETE:
         run_model_set_qr_read_complete(model, g_value_get_boolean( value ) );
         break;
      case RUN_MODEL_PROP_PERF_PRESS_CAL_COMPLETE:
         run_model_set_perf_press_cal_complete(model, g_value_get_boolean( value ) );
         break;
      case RUN_MODEL_PROP_PERF_FLOW_MEAS_COMPLETE:
         run_model_set_perf_flow_meas_complete(model, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void run_model_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_MODE:
         g_value_set_uint( value, run_model_get_run_mode( model ) );
         break;
      case RUN_MODEL_PROP_SETUP_STEP_CHANGE:
         g_value_set_uint( value, run_model_get_next_step( model ) );
         break;
      case RUN_MODEL_PROP_LEAK_CHECK_COMPLETE:
         g_value_set_boolean( value, run_model_get_leak_check_complete( model ) );
         break;
      case RUN_MODEL_PROP_QR_CODE_COMPLETE:
         g_value_set_boolean( value, run_model_get_qr_read_complete( model ) );
         break;
      case RUN_MODEL_PROP_PERF_PRESS_CAL_COMPLETE:
         g_value_set_boolean( value, run_model_get_perf_press_cal_complete( model ) );
         break;
      case RUN_MODEL_PROP_PERF_FLOW_MEAS_COMPLETE:
         g_value_set_boolean( value, run_model_get_perf_flow_meas_complete( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static guint      run_model_sigs[RUN_MODEL_N_SIGNALS] = {0, };
static GParamSpec *model_properties[RUN_MODEL_N_PROPERTIES] = {NULL, };

static void run_model_class_init( RunModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = run_model_finalize;

   gobject_class->get_property = run_model_get_property;
   gobject_class->set_property = run_model_set_property;

   model_properties[RUN_MODEL_PROP_RUN_MODE] = g_param_spec_uint(RUN_MODEL_MODE_CHANGE_PROP_STR, NULL, NULL,
                                                                 RUN_MODE_NOT_SET, N_RUM_MODEL_MODES-1, RUN_MODE_NOT_SET, G_PARAM_READWRITE  | G_PARAM_STATIC_NAME);
   model_properties[RUN_MODEL_PROP_SETUP_STEP_CHANGE] = g_param_spec_uint(RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR, NULL, NULL,
                                                                          RUN_SETUP_UNINITIALIZED, RUN_SETUP_N_STEPS-1, RUN_SETUP_UNINITIALIZED, G_PARAM_READWRITE  | G_PARAM_STATIC_NAME);
   model_properties[RUN_MODEL_PROP_LEAK_CHECK_COMPLETE] = g_param_spec_boolean(RUN_MODEL_SETUP_LEAK_COMPLETE_PROP_STR, NULL, NULL,
                                                                               FALSE, G_PARAM_READWRITE  | G_PARAM_STATIC_NAME);

   model_properties[RUN_MODEL_PROP_PUMPING] = g_param_spec_boolean(RM_PUMPING_PROP_STR, NULL, NULL,
                                                                   FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
   model_properties[RUN_MODEL_PROP_PERF_PRESS_CAL_COMPLETE] = g_param_spec_boolean(RM_PERF_PRESS_CAL_COMPLETE_PROP_STR, NULL, NULL,
                                                                                   FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
   model_properties[RUN_MODEL_PROP_PERF_FLOW_MEAS_COMPLETE] = g_param_spec_boolean(RM_PERF_FLOW_MEAS_COMPLETE_PROP_STR, NULL, NULL,
                                                                                   FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
   model_properties[RUN_MODEL_PROP_QR_CODE_COMPLETE] = g_param_spec_boolean(RM_QR_CODE_COMPLETE_PROP_STR, NULL, NULL,
                                                                            FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);

   g_object_class_install_properties( gobject_class, RUN_MODEL_N_PROPERTIES, model_properties);

   run_model_sigs[RUN_MODEL_SIGNAL_MODE_CHANGE] = g_signal_new_class_handler(RUN_MODEL_MODE_CHANGE_SIGNAL_STR, G_TYPE_FROM_CLASS(klass),
                                                                                        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION, (GCallback)run_model_listener_response,
                                                                                        NULL, NULL, g_cclosure_marshal_VOID__INT, G_TYPE_NONE, 1, G_TYPE_INT);
   run_model_sigs[RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE] = g_signal_new_class_handler(RUN_MODEL_SETUP_STEP_CHANGE_SIGNAL_STR, G_TYPE_FROM_CLASS(klass),
                                                                                        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION, (GCallback)run_step_listener_response,
                                                                                        NULL, NULL, g_cclosure_marshal_VOID__INT, G_TYPE_NONE, 1, G_TYPE_INT);
}

static void run_model_init(RunModel *self)
{
   RunModelPrivate *priv = run_model_get_instance_private(self);
}

RunModel *run_model_new()
{
   RunModel *model;
   model = g_object_new(RUN_TYPE_MODEL,
                        RUN_MODEL_MODE_CHANGE_PROP_STR, RUN_MODE_NOT_SET,
                        NULL);
   model->run_mode = RUN_MODE_NOT_SET;
   model->last_completed_phase = RUN_SETUP_UNINITIALIZED;
   model->leak_check_complete = FALSE;
   model->leak_check_success = FALSE;

   model->perf_press_cal_running = FALSE;
   model->perf_press_cal_complete = FALSE;
   model->perf_press_cal_success = FALSE;
   model->perf_flow_meas_running = FALSE;
   model->perf_flow_meas_complete = FALSE;
   model->perf_flow_meas_success = FALSE;

   return model;
}

RUN_MODEL_MODE run_model_get_run_mode( RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), RUN_MODE_NOT_SET);
   return self->run_mode;
}
void run_model_set_run_mode( RunModel *self, RUN_MODEL_MODE mode )
{
   if (self->run_mode != mode)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "%s: mode=%d", __func__, self->run_mode);

      self->run_mode = mode;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_RUN_MODE]);
      g_signal_emit(G_OBJECT(self), run_model_sigs[RUN_MODEL_SIGNAL_MODE_CHANGE], 0, self->run_mode);
   }
}

gchar *run_model_get_run_description(RunModel *self)
{
   return g_strdup(self->run_description);
}
void run_model_set_run_description(RunModel *self, const gchar *run_description)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   self->run_description = g_strdup(run_description);
}

void run_model_set_leak_check_complete(RunModel *self, gboolean complete)
{
   if (self->leak_check_complete != complete)
   {
      self->leak_check_complete = complete;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_LEAK_CHECK_COMPLETE]);
   }
}

gboolean run_model_get_pumping(RunModel *self)
{
   return self->pumping;
}
void run_model_set_pumping(RunModel *self, gboolean pumping)
{
   if(self->pumping != pumping)
   {
      self->pumping = pumping;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_PUMPING]);
   }
}

gboolean run_model_get_leak_check_result(RunModel *self)
{
   return self->leak_check_success;
}
void run_model_set_leak_check_result(RunModel *self, gboolean passed)
{
   self->leak_check_success = passed;
}
gboolean run_model_get_leak_check_complete(RunModel *self)
{
   return self->leak_check_complete;
}

gboolean run_model_get_qr_read_complete(RunModel *self)
{
   return self->qr_code_complete;
}
void run_model_set_qr_read_complete(RunModel *self, gboolean complete)
{
   if (self->qr_code_complete != complete)
   {
      self->qr_code_complete = TRUE;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_QR_CODE_COMPLETE]);
   }
}

guint run_model_get_blood_volume(RunModel *self)
{
   return self->fluid_volume;
}
gboolean run_model_validate_and_set_blood_volume(RunModel *self, const gchar *ui_input_str)
{
   if (ui_input_str == NULL || strcmp(ui_input_str, "\0") == 0)
   {
      return FALSE;
   }
   char *end_ptr;
   guint parsed_int = strtoul(ui_input_str, &end_ptr, 10);
   if (parsed_int == 0 && ui_input_str == end_ptr)
   {
      // failed to parse an int in string
      return FALSE;
   }
   else if (parsed_int < 10 || parsed_int > 100)
   {
      return FALSE;
   }
   else
   {
      self->fluid_volume = parsed_int;
      return TRUE;
   }
}

gboolean run_model_get_perf_press_cal_result(RunModel *self)
{
   return self->perf_press_cal_success;
}
void run_model_set_perf_press_cal_result(RunModel *self, gboolean passed)
{
   self->perf_press_cal_success = passed;
}
gboolean run_model_get_perf_press_cal_complete(RunModel *self)
{
   return self->perf_press_cal_complete;
}
void run_model_set_perf_press_cal_complete(RunModel *self, gboolean complete)
{
   if (self->perf_press_cal_running && self->perf_press_cal_complete != complete)
   {
      self->perf_press_cal_complete = complete;
      self->perf_press_cal_running = FALSE;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_PERF_PRESS_CAL_COMPLETE]);
   }
}

gboolean run_model_get_perf_flow_meas_result(RunModel *self)
{
   return self->perf_flow_meas_success;
}
void run_model_set_perf_flow_meas_result(RunModel *self, gboolean passed)
{
   self->perf_flow_meas_success = passed;
}
gboolean run_model_get_perf_flow_meas_complete(RunModel *self)
{
   return self->perf_flow_meas_complete;
}
void run_model_set_perf_flow_meas_complete(RunModel *self, gboolean complete)
{
   if (self->perf_flow_meas_running && self->perf_flow_meas_complete != complete)
   {
      self->perf_flow_meas_complete = complete;
      self->perf_flow_meas_running = FALSE;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_PERF_FLOW_MEAS_COMPLETE]);
   }
}


// Command interface
void run_model_cmd_start_leak_check(RunModel *self)
{
//   run_model_set_leak_check_running(self, TRUE);
   self->leak_check_running = TRUE;
   g_print("%s: start pneu test\n");
}
void run_model_cmd_load_albumin(RunModel *self)
{
   run_model_cmd_start_pumping(self);
}
void run_model_cmd_start_pumping(RunModel *self)
{
   g_print("%s: START PUMPS", __func__);
   run_model_set_pumping(self, TRUE);
}
void run_model_cmd_stop_pumping(RunModel *self)
{
   g_print("%s: STOP PUMPS", __func__);
   run_model_set_pumping(self, FALSE);
}
void run_model_cmd_start_perf_press_cal_check(RunModel *self)
{
   g_print("%s: START PERF PRESS CHECK", __func__);
   run_model_cmd_stop_pumping(self);
   self->perf_press_cal_running = TRUE;
}
void run_model_cmd_start_perf_flow_meas_check(RunModel *self)
{
   g_print("%s: START PERF FLOW CHECK", __func__);
   self->perf_flow_meas_running = TRUE;
}
void run_model_cmd_start_priming_nutrition_tubing(RunModel *self)
{
   g_print("%s: START PRIMING INFUSION TUBING", __func__);
}
void run_model_cmd_set_blood_params(RunModel *self)
{
   g_print("%s: SET PUMPING AND HEATING FOR BLOOD", __func__);
}

void run_model_cmd_start_blood_oxygenation(RunModel *self)
{
   g_print("%s: START BLOOD OXYGENATION", __func__);
}

guint run_model_get_fluid_volume(RunModel *self)
{
   return self->fluid_volume;
}
gboolean run_model_validate_and_set_fluid_volume(RunModel *self, const gchar *ui_input_str)
{
   if (ui_input_str == NULL || strcmp(ui_input_str, "\0") == 0)
   {
      return FALSE;
   }
   char *end_ptr;
   guint parsed_int = strtoul(ui_input_str, &end_ptr, 10);
   if (parsed_int == 0 && ui_input_str == end_ptr)
   {
      // failed to parse an int in string
      return FALSE;
   }
   else if (parsed_int < 10 || parsed_int > 100)
   {
      return FALSE;
   }
   else
   {
      self->fluid_volume = parsed_int;
      return TRUE;
   }
}

RUN_SETUP_STEPS run_model_get_next_step(RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), RUN_SETUP_FAILED);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   return self->last_completed_phase;
}
void run_model_set_next_step(RunModel *self, RUN_SETUP_STEPS step )
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   if (self->last_completed_phase != step)
   {
      // ----------------------
      // This is where we do things on system that we've triggered from the setup menu
      self->last_completed_phase = step;

      switch(self->last_completed_phase)
      {
         case RUN_SETUP_UNINITIALIZED:
            break;
         case RUN_SETUP_MODE_SELECTED:
            logging_llprintf(LOGLEVEL_DEBUG, "%s: RUN_SETUP_MODE_SELECTED->start mem check", __func__);
            break;
         case RUN_SETUP_MEMCHECK_COMPLETE:
            break;
         case RUN_SETUP_INTERMEDIATE_STEPS:
            break;
         case RUN_SETUP_PROMPT_FOR_FLUID_VOLUME:
            break;
         case RUN_SETUP_COMPLETE:
            logging_llprintf(LOGLEVEL_DEBUG, "%s: RUN_SETUP_COMPLETE->switch to perfusion tab", __func__);
            break;
         case RUN_SETUP_FAILED:
         case RUN_SETUP_N_STEPS:
            logging_llprintf(LOGLEVEL_DEBUG, "%s: RUN_SETUP_FAILED->we failed or got a state we don't understand", __func__);
            break;
      }
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_SETUP_STEP_CHANGE]);
      g_signal_emit(G_OBJECT(self), run_model_sigs[RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE], 0, self->last_completed_phase);
   }
}
