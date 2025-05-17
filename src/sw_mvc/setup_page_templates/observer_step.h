/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef OBSERVER_STEP_H__
#define OBSERVER_STEP_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define OBSERVER_TYPE_STEP (observer_step_get_type())

G_DECLARE_FINAL_TYPE(ObserverStep, observer_step, OBSERVER, STEP, GtkBox)

ObserverStep *observer_step_new(const gchar *observer_description,
                                RunModel *model,
                                const gchar *model_property,
                                RUN_SETUP_STEPS on_success,
                                RUN_SETUP_STEPS on_failure,
                                RunModelBooleanGetterFn_T result_fn);

G_END_DECLS
#endif  // OBSERVER_STEP_H__
