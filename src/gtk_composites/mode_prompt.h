/**
 * Created by french on 10/5/24.
 * @brief
 */
#ifndef MODE_PROMPT_H__
#define MODE_PROMPT_H__
#include "app_widgets.h"
#include "gtk_composites/app_mode_selector.h"
G_BEGIN_DECLS

#define MODE_TYPE_PROMPT             ( mode_prompt_get_type () )
G_DECLARE_FINAL_TYPE(ModePrompt, mode_prompt, MODE, PROMPT, GtkBox )

typedef gboolean (*RunValidationCallback_T)(AppModeSelector *selector, const gchar *run_description, gpointer user_data);
typedef gboolean (*ServiceValidationCallback_T)(AppModeSelector *selector,const gchar *service_pass, const gchar *run_description, gpointer user_data);

GtkWidget* mode_prompt_new(AppModeSelector *mode_selector,
                           RunValidationCallback_T run_callback,
                           ServiceValidationCallback_T service_callback,
                           gpointer user_data);

G_END_DECLS
#endif  // MODE_PROMPT_H__
