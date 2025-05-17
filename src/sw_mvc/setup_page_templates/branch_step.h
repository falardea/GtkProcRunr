/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef BRANCH_STEP_H__
#define BRANCH_STEP_H__
#include <gtk/gtk.h>
#include "../run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define BRANCH_TYPE_STEP (branch_step_get_type())

G_DECLARE_FINAL_TYPE(BranchStep, branch_step, BRANCH, STEP, GtkBox)

BranchStep *branch_step_new(const gchar *branch_description,
                            RunModel *model,
                            const gchar *model_property,
                            RUN_SETUP_STEPS on_success);

G_END_DECLS
#endif  // BRANCH_STEP_H__
