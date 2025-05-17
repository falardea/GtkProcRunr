/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef RUN_VIEWER_H__
#define RUN_VIEWER_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define RUN_TYPE_VIEWER (run_viewer_get_type())

G_DECLARE_FINAL_TYPE(RunViewer, run_viewer, RUN, VIEWER, GtkBox)

RunViewer *run_viewer_new(RunModel *model);

void run_viewer_connect_model_signals(RunViewer *self, RunModel *model);

G_END_DECLS
#endif  // RUN_VIEWER_H__
