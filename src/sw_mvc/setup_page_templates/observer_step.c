/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "observer_step.h"
#include "template_common.h"
#include "utils/logging.h"

#define OBSERVER_STEP_PROCESS_COMPLETE_STR "process-complete"

struct _ObserverStep
{
   GtkBox      super;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_cancel;
   GBinding    *external_binding;

   RunModel          *model;
   gchar             *model_watch_prop;
   RUN_SETUP_STEPS   on_success;
   RUN_SETUP_STEPS   on_failure;
   RunModelBooleanGetterFn_T results_check_fn;

   gboolean          process_complete;
};

G_DEFINE_TYPE(ObserverStep, observer_step, GTK_TYPE_BOX)

static void observer_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(OBSERVER_IS_STEP(g_object));
   ObserverStep *self = OBSERVER_STEP(g_object);
   if (self->model_watch_prop)
   {
      g_free(self->model_watch_prop);
   }
   G_OBJECT_CLASS(observer_step_parent_class)->finalize(g_object);
}

void on_observer_step_btn_cancel_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: Not implemented, but could be", __func__);
}

static gboolean observer_step_get_process_complete(ObserverStep *self);
static void observer_step_set_process_complete(ObserverStep *self, gboolean complete);

enum
{
   PROP_0 = 0, // Reserved for GObject
   OBSERVER_STEP_PROCESS_COMPLETED,
   OBSERVER_STEP_N_PROPERTIES
};

static void observer_step_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   ObserverStep *self = OBSERVER_STEP(object);
   if (prop_id == OBSERVER_STEP_PROCESS_COMPLETED){
      observer_step_set_process_complete(self, g_value_get_boolean( value ) );
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void observer_step_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   ObserverStep *self = OBSERVER_STEP(object);
   if (prop_id == OBSERVER_STEP_PROCESS_COMPLETED)
   {
      g_value_set_boolean( value, observer_step_get_process_complete(self));
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[OBSERVER_STEP_N_PROPERTIES] = {NULL, };

static void observer_step_class_init(ObserverStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = observer_step_finalize;
   gobject_class->get_property = observer_step_get_property;
   gobject_class->set_property = observer_step_set_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/observer_step");
   gtk_widget_class_bind_template_child(widget_class, ObserverStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, ObserverStep, btn_cancel);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_clicked", (GCallback)on_observer_step_btn_cancel_clicked);

   model_properties[OBSERVER_STEP_PROCESS_COMPLETED] = g_param_spec_boolean(OBSERVER_STEP_PROCESS_COMPLETE_STR,
                                                                          OBSERVER_STEP_PROCESS_COMPLETE_STR,
                                                                          OBSERVER_STEP_PROCESS_COMPLETE_STR,
                                                                          FALSE, G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, OBSERVER_STEP_N_PROPERTIES, model_properties);

}

static void observer_step_init(ObserverStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}


/*
 static gboolean toggleButtonLabelsOnActive(GBinding *gBinding, const GValue *fromValue,
                                           __attribute__((unused)) GValue *toValue,
                                           gpointer user_data)
{
   TOGGLE_LABEL_TYPE tgl_type = GPOINTER_TO_UINT(user_data);
   if ((tgl_type >= NUM_TOGGLE_LABEL_TYPES) || (tgl_type < 0))
   {
      REPORT_BUG_DETAILS("Unrecognized TOGGLE_LABEL_TYPE: %d", tgl_type);
      return FALSE;
   }

   GtkToggleButton *tbutton = (GtkToggleButton *) g_binding_dup_source(gBinding);
   if ((tbutton != NULL) && (fromValue != NULL))
   {
      gboolean isActive = (gboolean) g_value_get_boolean(fromValue);
      gtk_button_set_label(GTK_BUTTON(tbutton), isActive ? tgl_label_arr[tgl_type].active_state : tgl_label_arr[tgl_type].inactive_state);
      g_object_unref(tbutton);
      return TRUE;
   }
   g_object_unref(tbutton);
   return FALSE;
}

 static void setToggleLabelBinding(GtkWidget *tbutton, TOGGLE_LABEL_TYPE toggleLabelType)
{
   if (tbutton != NULL)
   {
      g_object_bind_property_full(tbutton, "active", tbutton, "label",
                                  G_BINDING_SYNC_CREATE,
                                  (GBindingTransformFunc) (toggleButtonLabelsOnActive),
                                  NULL, GUINT_TO_POINTER(toggleLabelType), NULL);
   }
}

 */


ObserverStep *observer_step_new(const gchar *observer_description,
                                RunModel *model,
                                const gchar *model_property,
                                RUN_SETUP_STEPS on_success,
                                RUN_SETUP_STEPS on_failure,
                                RunModelBooleanGetterFn_T result_fn)
{
   ObserverStep *self;
   self = g_object_new(OBSERVER_TYPE_STEP, NULL);

   if (observer_description != NULL)
   {
      gtk_label_set_text(self->lbl_step_description, observer_description);
   }
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_cancel), FALSE);

   self->model = model;
   self->model_watch_prop = g_strdup(model_property);
   self->on_success = on_success;
   self->on_failure = on_failure;
   self->process_complete = FALSE;
   self->results_check_fn = result_fn;

   self->external_binding = g_object_bind_property(self->model, self->model_watch_prop,
                                                   self, OBSERVER_STEP_PROCESS_COMPLETE_STR, G_BINDING_DEFAULT);

   return self;
}

static gboolean observer_step_get_process_complete(ObserverStep *self)
{
   return self->process_complete;
}
static void observer_step_set_process_complete(ObserverStep *self, gboolean complete)
{
   if (self->process_complete != complete)
   {
      self->process_complete = complete;

      if (self->results_check_fn(self->model))
      {
         // On success ??
         // g_object_unref(self->external_binding);
         run_model_set_last_completed_step(self->model, self->on_success);
      }
      else
      {
         run_model_set_last_completed_step(self->model, self->on_failure);
      }

   }
}