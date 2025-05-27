#include "autocmd_step.h"
#include "utils/logging.h"

#define AUTOCMD_STEP_SHOW_STR "step-is-shown"

struct _AutocmdStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_description;

   RunModel          *model;
   RunModelCommandFn_T  cmd_fn;
   RUN_SETUP_STEPS   go_to_state;
};

G_DEFINE_TYPE(AutocmdStep, autocmd_step, GTK_TYPE_BOX)

static void autocmd_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(AUTOCMD_IS_STEP(g_object));
   G_OBJECT_CLASS(autocmd_step_parent_class)->finalize(g_object);
}

static void autocmd_step_class_init(AutocmdStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = autocmd_step_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/autocmd_step");
   gtk_widget_class_bind_template_child(widget_class, AutocmdStep, lbl_step_description);
}

static void autocmd_step_init(AutocmdStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

static gboolean issue_command_for_step(gpointer user_data)
{
   AutocmdStep *self = AUTOCMD_STEP(user_data);
   if (self->cmd_fn)
   {
      self->cmd_fn(self->model);
   }
   run_model_set_next_step(self->model, self->go_to_state);
   return G_SOURCE_REMOVE;
}

void autocmd_map_signal_handler(GtkWidget *source,__attribute__((unused)) gboolean sig_val,__attribute__((unused)) gpointer user_data)
{

   gdk_threads_add_idle(issue_command_for_step, source);
}

AutocmdStep *autocmd_step_new(const gchar *step_description,
                              RunModel *model,
                              RunModelCommandFn_T cmd_fn,
                              RUN_SETUP_STEPS state_to_set)
{
   AutocmdStep *self;
   self = g_object_new(AUTOCMD_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_step_description, step_description);

   self->model = model;
   self->go_to_state = state_to_set;
   self->cmd_fn = cmd_fn;

   g_signal_connect (self, "map", G_CALLBACK(autocmd_map_signal_handler), self);

   return self;
}
