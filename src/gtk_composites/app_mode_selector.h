/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef APP_MODE_SELECTOR_H__
#define APP_MODE_SELECTOR_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define APP_TYPE_MODE_SELECTOR (app_mode_selector_get_type())

G_DECLARE_FINAL_TYPE(AppModeSelector, app_mode_selector, APP, MODE_SELECTOR, GtkBox)

AppModeSelector *app_mode_selector_new(RunModel *model, GtkOverlay *popup_container);

G_END_DECLS
#endif  // APP_MODE_SELECTOR_H__
