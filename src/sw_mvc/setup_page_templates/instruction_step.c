/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "instruction_step.h"
#include "utils/logging.h"

struct _InstructionStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_description;
   GtkImage    *img_step_visual;

   GtkButton   *btn_start_next;

   RunModel          *model;
   RUN_SETUP_STEPS   go_to_state;
};

G_DEFINE_TYPE(InstructionStep, instruction_step, GTK_TYPE_BOX)

static void instruction_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(INSTRUCTION_IS_STEP(g_object));
   G_OBJECT_CLASS(instruction_step_parent_class)->finalize(g_object);
}

static void on_instruction_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   InstructionStep *self = INSTRUCTION_STEP(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   run_model_set_next_step(self->model, self->go_to_state);
}

static void instruction_step_class_init(InstructionStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = instruction_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/instruction_step");
   gtk_widget_class_bind_template_child(widget_class, InstructionStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, InstructionStep, img_step_visual);
   gtk_widget_class_bind_template_child(widget_class, InstructionStep, btn_start_next);


   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_instruction_step_btn_start_next_clicked);
}

static void instruction_step_init(InstructionStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

InstructionStep *instruction_step_new(const gchar *step_description,
                                      const gchar *next_label,
                                      const gchar *image_resource_str,
                                      RunModel *model,
                                      RUN_SETUP_STEPS state_to_set)
{
   InstructionStep *self;
   self = g_object_new(INSTRUCTION_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_step_description, step_description);
   gtk_button_set_label(self->btn_start_next, next_label != NULL ? next_label : "START");
   gtk_image_set_from_resource(self->img_step_visual, image_resource_str);

   self->model = model;
   self->go_to_state = state_to_set;

   return self;
}
