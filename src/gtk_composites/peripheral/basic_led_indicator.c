#include "basic_led_indicator.h"

#define BASIC_LED_INDICATOR_GREEN_ICON_PATH "/resource_path/LED_green"
#define BASIC_LED_INDICATOR_RED_ICON_PATH "/resource_path/LED_red"

G_DEFINE_TYPE(BasicLedIndicator, basic_led_indicator, GTK_TYPE_BOX)

static void basic_led_indicator_finalize( GObject *self )
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(BASIC_IS_LED_INDICATOR(self));

   G_OBJECT_CLASS(basic_led_indicator_parent_class)->finalize (self);
}

static void basic_led_indicator_class_init(BasicLedIndicatorClass *klass)
{
   GObjectClass *object_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   object_class->finalize = basic_led_indicator_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/basic_led_indicator");
   gtk_widget_class_bind_template_child(widget_class, BasicLedIndicator, indicator_image);
}

static void basic_led_indicator_init( BasicLedIndicator *self )
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *basic_led_indicator_new()
{
   BasicLedIndicator *wf;
   wf = g_object_new(BASIC_TYPE_LED_INDICATOR, NULL);

   basic_led_indicator_set_enabled(wf, FALSE);
   return GTK_WIDGET(wf);
}

/**
 * @ureq
 *  -# basic_led_indicator_set_connected() shall return immediately if the object
 *      passed in is NULL or if the object passed in is not a BasicLedIndicator
 *  -# basic_led_indicator_set_connected() shall set the connected member variable
 *      to enabled
 *  -# basic_led_indicator_set_connected() shall call gtk_image_set_from_resource with
 *      a reference to the indicator_image member variable, the string
 *      BASIC_CONNECTED_ICON_PATH if the connected member variable is TRUE
 *  -# basic_led_indicator_set_connected() shall call gtk_image_set_from_resource with
 *      a reference to the indicator_image member variable, the string
 *      BASIC_DISCONNECTED_ICON_PATH if the connected member variable is FALSE
 */
void basic_led_indicator_set_enabled(BasicLedIndicator *self, gboolean enabled)
{
   self->enabled = enabled;
   gtk_image_set_from_resource(self->indicator_image, self->enabled ? BASIC_LED_INDICATOR_GREEN_ICON_PATH : BASIC_LED_INDICATOR_RED_ICON_PATH);
}
