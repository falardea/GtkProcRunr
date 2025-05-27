#ifndef AUTOCMD_STEP_H__
#define AUTOCMD_STEP_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS

#define AUTOCMD_TYPE_STEP (autocmd_step_get_type())

G_DECLARE_FINAL_TYPE(AutocmdStep, autocmd_step, AUTOCMD, STEP, GtkBox)

AutocmdStep *autocmd_step_new(const gchar *step_description,
                              RunModel *model,
                              RunModelCommandFn_T cmd_fn,
                              RUN_SETUP_STEPS state_to_set);

G_END_DECLS
#endif  // AUTOCMD_STEP_H__
