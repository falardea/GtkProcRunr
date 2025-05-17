/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef COMMAND_STEP_H__
#define COMMAND_STEP_H__
#include <gtk/gtk.h>
#include "../setup_view.h"
#include "../run_model.h"
G_BEGIN_DECLS

#define COMMAND_TYPE_STEP (command_step_get_type())

G_DECLARE_FINAL_TYPE(CommandStep, command_step, COMMAND, STEP, GtkBox)

CommandStep *command_step_new(const gchar *step_description,
                              const gchar *next_label,
                              RunModel *model,
                              RUN_SETUP_STEPS state_to_set);

G_END_DECLS
#endif  // COMMAND_STEP_H__
