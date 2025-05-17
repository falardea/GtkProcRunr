/**
 * Created by french on 10/5/24.
 * @brief
 */
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "utils/logging.h"
#include "mode_prompt.h"

struct _ModePrompt
{
   GtkBox         parent;

   GtkEntry       *entry_run_description;
   GtkEntry       *entry_service_password;

   GtkCheckButton *ckbtn_enable_service;

   // GtkButton      *btn_start_run;
   // GtkButton      *btn_cancel;

   AppModeSelector               *mode_selector;
   RunValidationCallback_T       run_validator;
   ServiceValidationCallback_T   service_validator;
   gpointer                      user_callback_data;
};

G_DEFINE_TYPE(ModePrompt, mode_prompt, GTK_TYPE_BOX )

static void mode_prompt_finalize( GObject *oSelf )
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS (mode_prompt_parent_class)->finalize (oSelf);
}

static void on_ckbtn_enable_service_toggled(GtkToggleButton *enable_chkbx, gpointer user_data)
{
   ModePrompt *self = MODE_PROMPT(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(self->entry_service_password), gtk_toggle_button_get_active(enable_chkbx));
}

static void on_btn_start_run_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   ModePrompt *self = MODE_PROMPT(user_data);
   gtk_style_context_remove_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_run_description)), "error");
   gtk_style_context_remove_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_service_password)), "error");

   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->ckbtn_enable_service)))
   {
      gboolean valid_input = (self->service_validator)(self->mode_selector,
            gtk_entry_get_text(self->entry_service_password),
            gtk_entry_get_text(self->entry_run_description),
            self->user_callback_data);

      if(valid_input)
      {
         gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
      }
      else
      {
         // If the service toggle button is active, and we failed validation, we've got the wrong password
         gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_service_password)), "error");
         gtk_widget_grab_focus(GTK_WIDGET(self->entry_service_password));
      }
   }
   else
   {

      gboolean valid_input = (self->run_validator)(self->mode_selector,
            gtk_entry_get_text(self->entry_run_description),
            self->user_callback_data);
      if(valid_input)
      {
         gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
      }
      else
      {
         gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_run_description)), "error");
         gtk_widget_grab_focus(GTK_WIDGET(self->entry_run_description));
      }
   }
}

static void on_btn_cancel_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ModePrompt *self = MODE_PROMPT(user_data);
   /** (* self->validate_and_submit_callback)(GTK_RESPONSE_REJECT); */
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
}

static void mode_prompt_class_init(ModePromptClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = mode_prompt_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/mode_prompt");
   gtk_widget_class_bind_template_child(widget_class, ModePrompt, entry_run_description);
   gtk_widget_class_bind_template_child(widget_class, ModePrompt, entry_service_password);
   gtk_widget_class_bind_template_child(widget_class, ModePrompt, ckbtn_enable_service);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_enable_service_toggled", (GCallback)on_ckbtn_enable_service_toggled);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_run_clicked", (GCallback)on_btn_start_run_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_clicked", (GCallback)on_btn_cancel_clicked);
}

static void mode_prompt_init(ModePrompt *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *mode_prompt_new(AppModeSelector *mode_selector,
                           RunValidationCallback_T run_callback,
                           ServiceValidationCallback_T service_callback,
                           gpointer user_data)
{
   g_return_val_if_fail(run_callback != NULL, NULL);
   g_return_val_if_fail(service_callback != NULL, NULL);

   ModePrompt *self;
   self = g_object_new(MODE_TYPE_PROMPT, NULL);

   self->mode_selector = mode_selector;
   self->run_validator = run_callback;
   self->service_validator = service_callback;
   self->user_callback_data = user_data;

   return GTK_WIDGET(self);
}
