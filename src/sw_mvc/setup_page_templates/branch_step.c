/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "branch_step.h"
#include "utils/logging.h"

#define BRANCH_STEP_PROCESS_COMPLETE_STR "process-complete"

struct _BranchStep
{
   GtkBox      super;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_cancel;

   RunModel          *model;
   gchar             *model_watch_prop;
   RUN_SETUP_STEPS   on_success;

   gboolean          process_complete;
};

G_DEFINE_TYPE(BranchStep, branch_step, GTK_TYPE_BOX)

static void branch_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(BRANCH_IS_STEP(g_object));
   BranchStep *self = BRANCH_STEP(g_object);
   if (self->model_watch_prop)
   {
      g_free(self->model_watch_prop);
   }
   G_OBJECT_CLASS(branch_step_parent_class)->finalize(g_object);
}

void on_branch_step_btn_cancel_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: Not implemented, but could be", __func__);
}

static gboolean branch_step_get_process_complete(BranchStep *self);
static void branch_step_set_process_complete(BranchStep *self, gboolean complete);

enum
{
   PROP_0 = 0, // Reserved for GObject
   BRANCH_STEP_PROCESS_COMPLETED,
   BRANCH_STEP_N_PROPERTIES
};

static void branch_step_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   BranchStep *self = BRANCH_STEP(object);
   if (prop_id == BRANCH_STEP_PROCESS_COMPLETED){
      branch_step_set_process_complete(self, g_value_get_boolean( value ) );
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void branch_step_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   BranchStep *self = BRANCH_STEP(object);
   if (prop_id == BRANCH_STEP_PROCESS_COMPLETED)
   {
      g_value_set_boolean( value, branch_step_get_process_complete(self));
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[BRANCH_STEP_N_PROPERTIES] = {NULL, };

static void branch_step_class_init(BranchStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = branch_step_finalize;
   gobject_class->get_property = branch_step_get_property;
   gobject_class->set_property = branch_step_set_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/branch_step");
   gtk_widget_class_bind_template_child(widget_class, BranchStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, BranchStep, btn_cancel);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_clicked", (GCallback)on_branch_step_btn_cancel_clicked);

   model_properties[BRANCH_STEP_PROCESS_COMPLETED] = g_param_spec_boolean(BRANCH_STEP_PROCESS_COMPLETE_STR,
                                                                          BRANCH_STEP_PROCESS_COMPLETE_STR,
                                                                          BRANCH_STEP_PROCESS_COMPLETE_STR,
                                                                          FALSE, G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, BRANCH_STEP_N_PROPERTIES, model_properties);

}

static void branch_step_init(BranchStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

BranchStep *branch_step_new(const gchar *branch_description,
                            RunModel *model,
                            const gchar *model_property,
                            RUN_SETUP_STEPS on_success)
{
   BranchStep *self;
   self = g_object_new(BRANCH_TYPE_STEP, NULL);

   if (branch_description != NULL)
   {
      gtk_label_set_text(self->lbl_step_description, branch_description);
   }
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_cancel), FALSE);

   self->model = model;
   self->model_watch_prop = g_strdup(model_property);
   self->on_success = on_success;
   self->process_complete = FALSE;

   g_object_bind_property(self->model, self->model_watch_prop,
                          self, BRANCH_STEP_PROCESS_COMPLETE_STR, G_BINDING_DEFAULT);

   return self;
}

static gboolean branch_step_get_process_complete(BranchStep *self)
{
   return self->process_complete;
}
static void branch_step_set_process_complete(BranchStep *self, gboolean complete)
{
   if (self->process_complete != complete)
   {
      self->process_complete = complete;
      run_model_set_next_step(self->model, self->on_success);
   }
}