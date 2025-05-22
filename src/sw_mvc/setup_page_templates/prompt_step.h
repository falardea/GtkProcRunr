/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef PROMPT_STEP_H__
#define PROMPT_STEP_H__
#include <gtk/gtk.h>
#include "../run_model.h"

G_BEGIN_DECLS

#define PROMPT_TYPE_STEP (prompt_step_get_type())

G_DECLARE_FINAL_TYPE(PromptStep, prompt_step, PROMPT, STEP, GtkBox)

PromptStep *prompt_step_new(const gchar *step_description,
                            const gchar *right_btn_label,
                            const gchar *input_description,
                            const gchar *input_units,
                            const gchar *on_invalid_message,
                            RunModel *model,
                            RUN_SETUP_STEPS state_on_valid_input,
                            PromptStrValidateAndSetFn validation_callback);

G_END_DECLS
#endif  // PROMPT_STEP_H__
