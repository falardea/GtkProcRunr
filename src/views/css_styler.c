/**
 * @brief The implementation
 */
#include "css_styler.h"

void apply_app_styling(__attribute__((unused)) app_widgets *appWidgetsT) {
   GtkCssProvider *cssProvider = gtk_css_provider_new();

   gtk_css_provider_load_from_resource(cssProvider,
                                       "/resource_path/app_style");
   gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                             GTK_STYLE_PROVIDER(cssProvider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
   g_object_unref(cssProvider);
}
