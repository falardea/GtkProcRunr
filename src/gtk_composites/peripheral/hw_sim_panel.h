/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef HW_SIM_PANEL_H__
#define HW_SIM_PANEL_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"

G_BEGIN_DECLS // __cplusplus guard

#define HW_TYPE_SIM_PANEL (hw_sim_panel_get_type())

G_DECLARE_FINAL_TYPE(HwSimPanel, hw_sim_panel, HW, SIM_PANEL, GtkWindow)

HwSimPanel *hw_sim_panel_new(RunModel *model);

G_END_DECLS
#endif  // HW_SIM_PANEL_H__
