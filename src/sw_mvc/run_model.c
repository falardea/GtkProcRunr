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

   RUN_MODEL_MODE    run_mode;
   RUN_SETUP_STEPS   last_completed_phase;

   gboolean leak_check_complete;
   gboolean leak_check_success;
   PneumaticLeakTest *pneumatic_leak_test;

   gchar    *run_description;

   guint    fluid_volume;
};

G_DEFINE_TYPE_WITH_PRIVATE( RunModel, run_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   RUN_MODEL_PROP_RUN_MODE,
   RUN_MODEL_PROP_SETUP_STEP_CHANGE,
   RUN_MODEL_PROP_LEAK_CHECK_COMPLETE,
   RUN_MODEL_PROP_PNEUMATIC_LEAK_TEST,
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
         run_model_set_step( model, g_value_get_uint( value ) );
         break;
      case RUN_MODEL_PROP_LEAK_CHECK_COMPLETE:
         run_model_set_leak_check_complete(model, g_value_get_boolean( value ) );
         break;
      case RUN_MODEL_PROP_PNEUMATIC_LEAK_TEST:
         run_model_set_pneumatic_leak_test(model, g_value_get_boxed( value ) );
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
         g_value_set_uint( value, run_model_get_step( model ) );
         break;
      case RUN_MODEL_PROP_LEAK_CHECK_COMPLETE:
         g_value_set_boolean( value, run_model_get_leak_check_complete( model ) );
         break;
      case RUN_MODEL_PROP_PNEUMATIC_LEAK_TEST:
         g_value_set_boxed( value, run_model_get_pneumatic_leak_test( model ) );
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
   model_properties[RUN_MODEL_PROP_PNEUMATIC_LEAK_TEST] = g_param_spec_boxed(RUN_MODEL_PNEUMATIC_LEAK_TEST_PROP_STR, NULL, NULL,
                                                                             PNEUMATIC_TYPE_LEAK_TEST,
                                                                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME);

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

   model->pneumatic_leak_test = pneumatic_leak_test_new();
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

gboolean run_model_get_leak_check_complete(RunModel *self)
{
   return self->leak_check_complete;
}
void run_model_set_leak_check_complete(RunModel *self, gboolean complete)
{
   if (self->leak_check_complete != complete)
   {
      self->leak_check_complete = complete;
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_LEAK_CHECK_COMPLETE]);
   }
}

gboolean run_model_get_leak_check_success(RunModel *self)
{
   return self->pneumatic_leak_test->test_passed;
}
PneumaticLeakTest *run_model_get_pneumatic_leak_test(RunModel *self)
{
   return pneumatic_leak_test_copy(self->pneumatic_leak_test);
}
void run_model_set_pneumatic_leak_test(RunModel *self, PneumaticLeakTest *source)
{
   if (!pneumatic_leak_test_equal(self->pneumatic_leak_test, source))
   {
      g_free(self->pneumatic_leak_test);
      self->pneumatic_leak_test = pneumatic_leak_test_copy(source);
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_PNEUMATIC_LEAK_TEST]);
   }
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
   char *endptr;
   guint parsed_int = strtoul(ui_input_str, &endptr, 10);
   if (parsed_int == 0 && ui_input_str == endptr)
   {
      // failed to parse an int in string
      return FALSE;
   }
   else
   {
      self->fluid_volume = parsed_int;
      return TRUE;
   }
}

RUN_SETUP_STEPS run_model_get_step(RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), RUN_SETUP_FAILED);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   return self->last_completed_phase;
}
void run_model_set_step(RunModel *self, RUN_SETUP_STEPS step )
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

      // ----------------------
      g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_SETUP_STEP_CHANGE]);
      g_signal_emit(G_OBJECT(self), run_model_sigs[RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE], 0, self->last_completed_phase);
   }
}
