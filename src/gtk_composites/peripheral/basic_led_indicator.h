#ifndef BASIC_LED_INDICATOR_H__
#define BASIC_LED_INDICATOR_H__
#include "app_widgets.h"
G_BEGIN_DECLS

struct _BasicLedIndicator
{
   GtkBox parent;
   GtkImage *indicator_image;
   gboolean enabled;
};

#define BASIC_TYPE_LED_INDICATOR (basic_led_indicator_get_type())

G_DECLARE_FINAL_TYPE(BasicLedIndicator, basic_led_indicator, BASIC, LED_INDICATOR, GtkBox)

/**
 * @brief The wifi_indicator constructor
 * @return A newly constructed wifi_indicator
 */
GtkWidget *basic_led_indicator_new();

void basic_led_indicator_set_enabled(BasicLedIndicator *self, gboolean enabled);

G_END_DECLS
#endif  // BASIC_LED_INDICATOR_H__
