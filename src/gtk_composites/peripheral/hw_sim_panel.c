/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "hw_sim_panel.h"
#include "basic_led_indicator.h"
#include "utils/logging.h"

typedef struct
{
   BasicLedIndicator *test_mode_enabled;
}HwSimPanelPrivate;

struct _HwSimPanel
{
   GtkWindow      super;

   GtkBox         *test_mode_icon_box;

   GtkButton      *hw_sim_panel_close;
   GtkButton      *btn_memcheck_response;
   GtkButton      *btn_pump_enable_response;
   GtkButton      *btn_heat_enable_response;

   GtkCheckButton *ckbtn_memcheck_failure;
   GtkCheckButton *ckbtn_pump_enable_failure;
   GtkCheckButton *ckbtn_heat_enable_failure;

   GtkLabel       *lbl_run_id;

   RunModel *model;
};

G_DEFINE_TYPE_WITH_PRIVATE(HwSimPanel, hw_sim_panel, GTK_TYPE_WINDOW)

void hw_sim_panel_rx_mode_change(RunModel *source, RUN_MODEL_MODE mode, gpointer user_data);

static void hw_sim_panel_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   HwSimPanel *self = HW_SIM_PANEL(g_object);
   g_signal_handlers_disconnect_by_func(self->model, G_CALLBACK(hw_sim_panel_rx_mode_change), self);
   G_OBJECT_CLASS(hw_sim_panel_parent_class)->finalize(g_object);
}

static void on_hw_sim_panel_close_clicked(GtkWidget *button, gpointer *user_data);

static void on_btn_memcheck_response_clicked(GtkButton *button, gpointer *user_data);
static void on_btn_pump_enable_response_clicked(GtkButton *button, gpointer *user_data);
static void on_btn_heat_enable_response_clicked(GtkButton *button, gpointer *user_data);

static void on_ckbtn_memcheck_failure_toggled(GtkToggleButton *button, gpointer *user_data);
static void on_ckbtn_pump_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data);
static void on_ckbtn_heat_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data);

static void hw_sim_panel_class_init(HwSimPanelClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = hw_sim_panel_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/hw_sim_panel");
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, hw_sim_panel_close);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, test_mode_icon_box);

   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_memcheck_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_pump_enable_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_heat_enable_response);

   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_memcheck_failure);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_heat_enable_failure);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_pump_enable_failure);

   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, lbl_run_id);

   // Might be a bit overkill with all these close/destroy/delete... but we probably still aren't freeing the memory correctly for this
   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_close_clicked", (GCallback)on_hw_sim_panel_close_clicked);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_memcheck_response_clicked", (GCallback)on_btn_memcheck_response_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_pump_enable_response_clicked", (GCallback)on_btn_pump_enable_response_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_heat_enable_response_clicked", (GCallback)on_btn_heat_enable_response_clicked);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_memcheck_failure_toggled", (GCallback)on_ckbtn_memcheck_failure_toggled);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_pump_enable_failure_toggled", (GCallback)on_ckbtn_pump_enable_failure_toggled);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_heat_enable_failure_toggled", (GCallback)on_ckbtn_heat_enable_failure_toggled);
}

static void hw_sim_panel_init(HwSimPanel *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
}

HwSimPanel *hw_sim_panel_new(RunModel *model)
{
   HwSimPanel *self;
   self = g_object_new(HW_TYPE_SIM_PANEL, NULL);
   self->model = model;

   HwSimPanelPrivate *priv = hw_sim_panel_get_instance_private(self);
   priv->test_mode_enabled = BASIC_LED_INDICATOR(basic_led_indicator_new());
   gtk_box_pack_end(GTK_BOX(self->test_mode_icon_box), GTK_WIDGET(priv->test_mode_enabled), TRUE, TRUE, 0);

   g_signal_connect (G_OBJECT(model), RUN_MODEL_MODE_CHANGE_SIGNAL_STR, G_CALLBACK(hw_sim_panel_rx_mode_change), self);
   return self;
}

/////////////////////
static void on_btn_memcheck_response_clicked(__attribute__((unused))GtkButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_btn_pump_enable_response_clicked(__attribute__((unused))GtkButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   HwSimPanel *self = HW_SIM_PANEL(user_data);
   run_model_set_leak_check_complete(self->model, TRUE);
}
static void on_btn_heat_enable_response_clicked(__attribute__((unused))GtkButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void on_ckbtn_memcheck_failure_toggled(__attribute__((unused))GtkToggleButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_ckbtn_pump_enable_failure_toggled(__attribute__((unused))GtkToggleButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_ckbtn_heat_enable_failure_toggled(__attribute__((unused))GtkToggleButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

void hw_sim_panel_rx_mode_change(__attribute__((unused))RunModel *source, RUN_MODEL_MODE mode, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: >>>>>>>>>>>> run-mode = %d", __func__, mode);

   HwSimPanel *self = HW_SIM_PANEL(user_data);
   HwSimPanelPrivate *priv = hw_sim_panel_get_instance_private(self);

   basic_led_indicator_set_enabled(priv->test_mode_enabled, mode == RUN_MODE_TEST);
}

static void on_hw_sim_panel_close_clicked(GtkWidget *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_window_close(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
}
