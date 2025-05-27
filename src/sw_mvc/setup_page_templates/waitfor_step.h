/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef WAITFOR_STEP_H__
#define WAITFOR_STEP_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define WAITFOR_TYPE_STEP (waitfor_step_get_type())

G_DECLARE_FINAL_TYPE(WaitforStep, waitfor_step, WAITFOR, STEP, GtkBox)

WaitforStep *waitfor_step_new(const gchar *waitfor_description,
                              RunModel *model,
                              const gchar *model_property,
                              RUN_SETUP_STEPS on_next);

G_END_DECLS
#endif  // WAITFOR_STEP_H__
