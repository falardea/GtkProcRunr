#ifndef SETUP_VIEW_H__
#define SETUP_VIEW_H__
#include <gtk/gtk.h>
#include "run_model.h"
G_BEGIN_DECLS

#define SETUP_TYPE_VIEWER (setup_viewer_get_type())

G_DECLARE_FINAL_TYPE(SetupViewer, setup_viewer, SETUP, VIEWER, GtkBox)

typedef void (*CommandCallback_T)(RunModel *model, RUN_SETUP_STEPS next_state);

SetupViewer *setup_viewer_new(RunModel *model);

void setup_viewer_add_page_by_state_id(SetupViewer *self, GtkWidget *page, RUN_SETUP_STEPS step_id);
void setup_viewer_set_page_view(SetupViewer *self, RUN_SETUP_STEPS step_id);

G_END_DECLS
#endif  // SETUP_VIEW_H__
