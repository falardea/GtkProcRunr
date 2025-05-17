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

typedef gboolean (*PromptStepValidationFn)(PromptStep *self, GtkEntry *entry_widget);

PromptStep *prompt_step_new(const gchar *step_description,
                            const gchar *left_btn_label,
                            const gchar *right_btn_label,
                            const gchar *prompt_description,
                            const gchar *prompt_units,
                            RunModel *model,
                            RUN_SETUP_STEPS state_on_valid_input,
                            PromptStepValidationFn validation_callback);

G_END_DECLS
#endif  // PROMPT_STEP_H__
