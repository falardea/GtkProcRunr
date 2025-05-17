/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "command_step.h"
#include "template_common.h"
#include "utils/logging.h"


struct _CommandStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;

   RunModel          *model;
   RUN_SETUP_STEPS   go_to_state;
};

G_DEFINE_TYPE(CommandStep, command_step, GTK_TYPE_BOX)

static void command_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(COMMAND_IS_STEP(g_object));
   G_OBJECT_CLASS(command_step_parent_class)->finalize(g_object);
}

static void on_command_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   CommandStep *self = COMMAND_STEP(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   run_model_set_last_completed_step(self->model, self->go_to_state);
}

static void command_step_class_init(CommandStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = command_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/command_step");
   gtk_widget_class_bind_template_child(widget_class, CommandStep, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, btn_start_next);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_command_step_btn_start_next_clicked);
}

static void command_step_init(CommandStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

CommandStep *command_step_new(const gchar *step_description,
                              const gchar *next_label,
                              RunModel *model,
                              RUN_SETUP_STEPS state_to_set)
{
   CommandStep *self;
   self = g_object_new(COMMAND_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_step_description, step_description);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   gtk_button_set_label(self->btn_start_next, next_label != NULL ? next_label : "NEXT");

   self->model = model;
   self->go_to_state = state_to_set;

   return self;
}
