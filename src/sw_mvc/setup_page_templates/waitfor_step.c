/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "waitfor_step.h"
#include "utils/logging.h"

#define WAITFOR_STEP_PROCESS_COMPLETE_STR "process-complete"

struct _WaitforStep
{
   GtkBox      super;
   GtkLabel    *lbl_step_description;

   GBinding    *external_binding;

   RunModel          *model;
   gchar             *model_watch_prop;
   RUN_SETUP_STEPS   on_next;

   gboolean          process_complete;
};

G_DEFINE_TYPE(WaitforStep, waitfor_step, GTK_TYPE_BOX)

static void waitfor_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(WAITFOR_IS_STEP(g_object));
   WaitforStep *self = WAITFOR_STEP(g_object);
   if (self->model_watch_prop)
   {
      g_free(self->model_watch_prop);
   }
   G_OBJECT_CLASS(waitfor_step_parent_class)->finalize(g_object);
}

static gboolean waitfor_step_get_process_complete(WaitforStep *self);
static void waitfor_step_set_process_complete(WaitforStep *self, gboolean complete);

enum
{
   PROP_0 = 0, // Reserved for GObject
   WAITFOR_STEP_PROCESS_COMPLETED,
   WAITFOR_STEP_N_PROPERTIES
};

static void waitfor_step_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   WaitforStep *self = WAITFOR_STEP(object);
   if (prop_id == WAITFOR_STEP_PROCESS_COMPLETED){
      waitfor_step_set_process_complete(self, g_value_get_boolean( value ) );
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void waitfor_step_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   WaitforStep *self = WAITFOR_STEP(object);
   if (prop_id == WAITFOR_STEP_PROCESS_COMPLETED)
   {
      g_value_set_boolean( value, waitfor_step_get_process_complete(self));
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[WAITFOR_STEP_N_PROPERTIES] = {NULL, };

static void waitfor_step_class_init(WaitforStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = waitfor_step_finalize;
   gobject_class->get_property = waitfor_step_get_property;
   gobject_class->set_property = waitfor_step_set_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/waitfor_step");
   gtk_widget_class_bind_template_child(widget_class, WaitforStep, lbl_step_description);

   model_properties[WAITFOR_STEP_PROCESS_COMPLETED] = g_param_spec_boolean(WAITFOR_STEP_PROCESS_COMPLETE_STR,
                                                                            NULL, NULL, FALSE, G_PARAM_READWRITE );

   g_object_class_install_properties( gobject_class, WAITFOR_STEP_N_PROPERTIES, model_properties);

}

static void waitfor_step_init(WaitforStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

WaitforStep *waitfor_step_new(const gchar *waitfor_description,
                              RunModel *model,
                              const gchar *model_property,
                              RUN_SETUP_STEPS on_next)
{
   WaitforStep *self;
   self = g_object_new(WAITFOR_TYPE_STEP, NULL);

   if (waitfor_description != NULL)
   {
      gtk_label_set_text(self->lbl_step_description, waitfor_description);
   }

   self->model = model;
   self->model_watch_prop = g_strdup(model_property);
   self->on_next = on_next;
   self->process_complete = FALSE;

   self->external_binding = g_object_bind_property(self->model, self->model_watch_prop,
                                                   self, WAITFOR_STEP_PROCESS_COMPLETE_STR, G_BINDING_DEFAULT);

   return self;
}

static gboolean waitfor_step_get_process_complete(WaitforStep *self)
{
   return self->process_complete;
}
static void waitfor_step_set_process_complete(WaitforStep *self, gboolean complete)
{
   if (self->process_complete != complete)
   {
      self->process_complete = complete;
      run_model_set_next_step(self->model, self->on_next);
   }
}
