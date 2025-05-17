/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef UCHOICE_STEP_H__
#define UCHOICE_STEP_H__
#include <gtk/gtk.h>
#include "../run_model.h"
G_BEGIN_DECLS

#define UCHOICE_TYPE_STEP (uchoice_step_get_type())

G_DECLARE_FINAL_TYPE(UchoiceStep, uchoice_step, UCHOICE, STEP, GtkBox)

UchoiceStep *uchoice_step_new(const gchar *uchoice_description,
                              const gchar *left_btn_label,
                              const gchar *right_btn_label,
                              RunModel *model,
                              RUN_SETUP_STEPS state_to_left,
                              RUN_SETUP_STEPS state_to_right);

G_END_DECLS
#endif  // UCHOICE_STEP_H__
