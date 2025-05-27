/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef INSTRUCTION_STEP_H__
#define INSTRUCTION_STEP_H__
#include <gtk/gtk.h>
#include "../setup_view.h"
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS

#define INSTRUCTION_TYPE_STEP (instruction_step_get_type())

G_DECLARE_FINAL_TYPE(InstructionStep, instruction_step, INSTRUCTION, STEP, GtkBox)

InstructionStep *instruction_step_new(const gchar *step_description,
                                      const gchar *next_label,
                                      const gchar *image_resource_str,
                                      RunModel *model,
                                      RUN_SETUP_STEPS state_to_set);

G_END_DECLS
#endif  // INSTRUCTION_STEP_H__
