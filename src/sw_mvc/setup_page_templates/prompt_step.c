/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "prompt_step.h"
#include "utils/logging.h"

struct _PromptStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;

   GtkLabel    *lbl_invalid_entry_message;

   GtkLabel    *lbl_input_description;
   GtkEntry    *entry_input;
   GtkLabel    *lbl_input_units;

   RunModel                *model;
   RUN_SETUP_STEPS         next_on_valid;
   PromptStrValidateAndSetFn  validation_fn;
};

G_DEFINE_TYPE(PromptStep, prompt_step, GTK_TYPE_BOX)

static void prompt_step_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(PROMPT_IS_STEP(g_object));
   G_OBJECT_CLASS(prompt_step_parent_class)->finalize(g_object);
}

void on_prompt_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   PromptStep *self = PROMPT_STEP(user_data);

   gtk_style_context_remove_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_input)), "error");
   gtk_widget_set_visible(GTK_WIDGET(self->lbl_invalid_entry_message), FALSE);

   if(self->validation_fn(self->model, gtk_entry_get_text(self->entry_input)))
   {
      run_model_set_next_step(self->model, self->next_on_valid);
   }
   else
   {
      gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_input)), "error");
      gtk_widget_set_visible(GTK_WIDGET(self->lbl_invalid_entry_message), TRUE);
   }
}

static void prompt_step_class_init(PromptStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = prompt_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/prompt_step");
   gtk_widget_class_bind_template_child(widget_class, PromptStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, lbl_invalid_entry_message);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, lbl_input_description);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, entry_input);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, lbl_input_units);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_prompt_step_btn_start_next_clicked);
}

static void prompt_step_init(PromptStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

PromptStep *prompt_step_new(const gchar *step_description,
                            const gchar *right_btn_label,
                            const gchar *input_description,
                            const gchar *input_units,
                            const gchar *on_invalid_message,
                            RunModel *model,
                            RUN_SETUP_STEPS state_on_valid_input,
                            PromptStrValidateAndSetFn validation_callback)
{
   PromptStep *self;
   self = g_object_new(PROMPT_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_step_description, step_description);
   gtk_label_set_text(self->lbl_invalid_entry_message, on_invalid_message);
   gtk_label_set_text(self->lbl_input_description, input_description);
   gtk_label_set_text(self->lbl_input_units, input_units);

   gtk_button_set_label(self->btn_start_next, right_btn_label != NULL ? right_btn_label : "NEXT");

   self->model = model;
   self->next_on_valid = state_on_valid_input;
   self->validation_fn = validation_callback;

   return self;
}
