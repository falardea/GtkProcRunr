#ifndef CMDOBSERVER_STEP_H__
#define CMDOBSERVER_STEP_H__
#include <gtk/gtk.h>
#include "../setup_view.h"
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS

#define CMDOBSERVER_TYPE_STEP (cmdobserver_step_get_type())

G_DECLARE_FINAL_TYPE(CmdobserverStep, cmdobserver_step, CMDOBSERVER, STEP, GtkBox)

CmdobserverStep *cmdobserver_step_new(const gchar *cmd_step_description, const gchar *observer_step_description,
                                      const gchar *start_btn_label, const gchar *abort_btn_label,
                                      const gchar *model_property,
                                      RunModel *model,
                                      RunModelCommandFn_T cmd_fn,
                                      RUN_SETUP_STEPS on_success,
                                      RUN_SETUP_STEPS on_failure,
                                      RunModelBooleanGetterFn_T result_fn);

G_END_DECLS
#endif  // CMDOBSERVER_STEP_H__
