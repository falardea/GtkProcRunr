/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "uchoice_step.h"
#include "template_common.h"
#include "utils/logging.h"

struct _UchoiceStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;

   RunModel          *model;
   RUN_SETUP_STEPS   left_state;
   RUN_SETUP_STEPS   right_state;
};

G_DEFINE_TYPE(UchoiceStep, uchoice_step, GTK_TYPE_BOX)

static void uchoice_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(UCHOICE_IS_STEP(g_object));
   G_OBJECT_CLASS(uchoice_step_parent_class)->finalize(g_object);
}

void on_uchoice_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   UchoiceStep *self = UCHOICE_STEP(user_data);
   run_model_set_step(self->model, self->right_state);
}

void on_uchoice_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   UchoiceStep *self = UCHOICE_STEP(user_data);
   run_model_set_step(self->model, self->left_state);
}

static void uchoice_step_class_init(UchoiceStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = uchoice_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/uchoice_step");
   gtk_widget_class_bind_template_child(widget_class, UchoiceStep, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, UchoiceStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, UchoiceStep, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, UchoiceStep, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_uchoice_step_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_uchoice_step_btn_cancel_skip_clicked);
}

static void uchoice_step_init(UchoiceStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

UchoiceStep *uchoice_step_new(const gchar *uchoice_description,
                              const gchar *left_btn_label,
                              const gchar *right_btn_label,
                              RunModel *model,
                              RUN_SETUP_STEPS state_to_left,
                              RUN_SETUP_STEPS state_to_right)
{
   UchoiceStep *self;
   self = g_object_new(UCHOICE_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_step_description, uchoice_description);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   gtk_button_set_label(self->btn_start_next, right_btn_label != NULL ? right_btn_label : "NEXT");
   gtk_button_set_label(self->btn_cancel_skip, left_btn_label != NULL ? left_btn_label : "CANCEL");
   self->model = model;
   self->left_state = state_to_left;
   self->right_state = state_to_right;

   return self;
}
