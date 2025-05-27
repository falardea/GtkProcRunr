#include "cmdobserver_step.h"
#include "utils/logging.h"

#define CMDOBSERVER_STEP_PROCESS_COMPLETE_STR "process-complete"

struct _CmdobserverStep
{
   GtkBox      parent;

   GtkBox      *cmd_context;
   GtkBox      *observer_context;

   GtkLabel    *lbl_cmd_step_description;
   GtkLabel    *lbl_observer_step_description;

   GtkButton   *btn_cmd_start;
   GtkButton   *btn_observer_abort;

   GBinding    *external_binding;

   RunModel                   *model;
   gchar                      *model_watch_prop;
   RUN_SETUP_STEPS            on_success;
   RUN_SETUP_STEPS            on_failure;
   RunModelCommandFn_T        cmd_fn;
   RunModelBooleanGetterFn_T  results_check_fn;

   gboolean                   process_complete;
};

G_DEFINE_TYPE(CmdobserverStep, cmdobserver_step, GTK_TYPE_BOX)

static gboolean cmdobserver_step_get_process_complete(CmdobserverStep *self);
static void cmdobserver_step_set_process_complete(CmdobserverStep *self, gboolean complete);

static void cmdobserver_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(CMDOBSERVER_IS_STEP(g_object));
   CmdobserverStep *self = CMDOBSERVER_STEP(g_object);
   if (self->model_watch_prop)
   {
      g_free(self->model_watch_prop);
   }
   G_OBJECT_CLASS(cmdobserver_step_parent_class)->finalize(g_object);
}

static void on_btn_cmd_start_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   CmdobserverStep *self = CMDOBSERVER_STEP(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   gtk_widget_hide(GTK_WIDGET(self->cmd_context));
   gtk_widget_show(GTK_WIDGET(self->observer_context));

   self->process_complete = FALSE;

   self->external_binding = g_object_bind_property(self->model, self->model_watch_prop,
                                                   self, CMDOBSERVER_STEP_PROCESS_COMPLETE_STR, G_BINDING_DEFAULT);

   self->cmd_fn(self->model);
}

void on_btn_observer_abort_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: Not implemented, but could be", __func__);
}

enum
{
   PROP_0 = 0, // Reserved for GObject
   CMDOBSERVER_STEP_PROCESS_COMPLETED,
   CMDOBSERVER_STEP_N_PROPERTIES
};

static void cmdobserver_step_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   CmdobserverStep *self = CMDOBSERVER_STEP(object);
   if (prop_id == CMDOBSERVER_STEP_PROCESS_COMPLETED){
      cmdobserver_step_set_process_complete(self, g_value_get_boolean( value ) );
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void cmdobserver_step_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   CmdobserverStep *self = CMDOBSERVER_STEP(object);
   if (prop_id == CMDOBSERVER_STEP_PROCESS_COMPLETED)
   {
      g_value_set_boolean( value, cmdobserver_step_get_process_complete(self));
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[CMDOBSERVER_STEP_N_PROPERTIES] = {NULL, };

static void cmdobserver_step_class_init(CmdobserverStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = cmdobserver_step_finalize;
   gobject_class->get_property = cmdobserver_step_get_property;
   gobject_class->set_property = cmdobserver_step_set_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/cmdobserver_step");
   gtk_widget_class_bind_template_child(widget_class, CmdobserverStep, cmd_context);
   gtk_widget_class_bind_template_child(widget_class, CmdobserverStep, observer_context);

   gtk_widget_class_bind_template_child(widget_class, CmdobserverStep, lbl_cmd_step_description);
   gtk_widget_class_bind_template_child(widget_class, CmdobserverStep, lbl_observer_step_description);

   gtk_widget_class_bind_template_child(widget_class, CmdobserverStep, btn_cmd_start);
   gtk_widget_class_bind_template_child(widget_class, CmdobserverStep, btn_observer_abort);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cmd_start_clicked", (GCallback)on_btn_cmd_start_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_observer_abort_clicked", (GCallback)on_btn_observer_abort_clicked);

   model_properties[CMDOBSERVER_STEP_PROCESS_COMPLETED] = g_param_spec_boolean(CMDOBSERVER_STEP_PROCESS_COMPLETE_STR, NULL, NULL,
                                                                            FALSE, G_PARAM_READWRITE );

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_object_class_install_properties( gobject_class, CMDOBSERVER_STEP_N_PROPERTIES, model_properties);
}

static void cmdobserver_step_init(CmdobserverStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

CmdobserverStep *cmdobserver_step_new(const gchar *cmd_step_description, const gchar *observer_step_description,
                                      const gchar *start_btn_label, const gchar *abort_btn_label,
                                      const gchar *model_property,
                                      RunModel *model,
                                      RunModelCommandFn_T cmd_fn,
                                      RUN_SETUP_STEPS on_success,
                                      RUN_SETUP_STEPS on_failure,
                                      RunModelBooleanGetterFn_T result_fn)
{
   CmdobserverStep *self;
   self = g_object_new(CMDOBSERVER_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_cmd_step_description, cmd_step_description);
   gtk_label_set_text(self->lbl_observer_step_description, observer_step_description);
   gtk_button_set_label(self->btn_cmd_start, start_btn_label != NULL ? start_btn_label : "Start");
   gtk_button_set_label(self->btn_observer_abort, abort_btn_label != NULL ? abort_btn_label : "Abort");

   gtk_widget_show(GTK_WIDGET(self->cmd_context));
   gtk_widget_hide(GTK_WIDGET(self->observer_context));

   self->model = model;
   self->model_watch_prop = g_strdup(model_property);

   self->cmd_fn = cmd_fn;
   self->results_check_fn = result_fn;

   self->on_success = on_success;
   self->on_failure = on_failure;
   self->process_complete = FALSE;

   return self;
}


static gboolean cmdobserver_step_get_process_complete(CmdobserverStep *self)
{
   return self->process_complete;
}
static void cmdobserver_step_set_process_complete(CmdobserverStep *self, gboolean complete)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   if (self->process_complete != complete)
   {
      g_object_unref(self->external_binding);

      self->process_complete = complete;
      if (self->results_check_fn(self->model))
      {
         run_model_set_next_step(self->model, self->on_success);
      }
      else
      {
         run_model_set_next_step(self->model, self->on_failure);
      }
   }
   gtk_widget_show(GTK_WIDGET(self->cmd_context));
   gtk_widget_hide(GTK_WIDGET(self->observer_context));
}
