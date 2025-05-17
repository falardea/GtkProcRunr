/**
 * @brief The implementation
 */
#include "root_window.h"
#include "pseudo_app_model/app_interface.h"
#include "utils/sys_interface.h"
#include "utils/logging.h"
#include "gtk_composites/peripheral/log_terminal.h"
#include "gtk_composites/mode_prompt.h"
#include "gtk_composites/peripheral/hw_sim_panel.h"


static const char *MSG_OUT_CURSOR_NAME = "msgOutCursor";
static GtkTextMark *msgOutCursor;
static char timestamp[20];  // not sure why it felt better to allocate the memory once


void on_main_wnd_close_clicked(__attribute__((unused)) GtkWidget *srcWidget,
                                __attribute__((unused)) gpointer uData) {
   GtkWidget *parent_wnd = gtk_widget_get_toplevel(srcWidget);

   app_widgets *wdgts = get_app_widgets_pointer();
   if (wdgts->hw_sim_panel != NULL && GTK_IS_WIDGET(wdgts->hw_sim_panel))
   {
      gtk_widget_destroy(GTK_WIDGET(wdgts->hw_sim_panel));
   }

   gtk_widget_destroy(parent_wnd);
   gtk_main_quit();
}

gboolean on_main_wnd_delete_event(__attribute__((unused)) GtkWidget *srcWidget,
                                    __attribute__((unused)) GdkEvent *event,
                                    __attribute__((unused)) gpointer uData) {
   app_widgets *wdgts = get_app_widgets_pointer();
   if (wdgts->hw_sim_panel != NULL && GTK_IS_WIDGET(wdgts->hw_sim_panel))
   {
      gtk_widget_destroy(GTK_WIDGET(wdgts->hw_sim_panel));
   }

   gtk_main_quit();
   return FALSE;
}

void on_do_something_button_clicked(__attribute__((unused)) GtkButton *button, __attribute__((unused)) gpointer *user_data)
{
   app_widgets *wdgts = (app_widgets *) user_data;
   if (gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(wdgts->w_say_something_entry))) > 0){
      print_log_level_msgout(LOGLEVEL_INFO, "%s", gtk_entry_get_text(GTK_ENTRY(wdgts->w_say_something_entry)));
   } else {
      print_log_level_msgout(LOGLEVEL_INFO, "nothing to say?");
   }

   /*GtkWidget *dialog;
   dialog = gtk_message_dialog_new(GTK_WINDOW(wdgts->main_wnd), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Your message here");

   // Set the dialog to be transient for the parent window to ensure it stays on top of the parent
   gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(wdgts->main_wnd));

   // Set the dialog position to be centered on the parent window
   gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

   // Optionally, set a specific position
   // gtk_window_move(GTK_WINDOW(dialog), x_position, y_position);

   gtk_dialog_run(GTK_DIALOG(dialog));
   gtk_widget_destroy(dialog);*/

   if (wdgts->hw_sim_panel == NULL || !HW_IS_SIM_PANEL(wdgts->hw_sim_panel))
   {
      wdgts->hw_sim_panel = hw_sim_panel_new(wdgts->run_model);
   }
   gtk_widget_show(GTK_WIDGET(wdgts->hw_sim_panel));
}


//////////////////////////////////////////////////////

void print_log_level_msgout(LOGLEVEL loglevel, const char *_format, ...)
{
   if (loglevel >= get_app_log_level()) {
      app_widgets *wdgts = get_app_widgets_pointer();

      bool use_ts = get_app_log_w_timestamp_flag();

      // We're not going to "flag" out the timestamp in the memory sizing here, because... who cares if it's too big?
      char ll_msg_out[LOGGING_MAX_MSG_LENGTH + sizeof(timestamp) + (2*sizeof(':')) + sizeof(get_log_level_str(loglevel))];
      char line_out[LOGGING_MAX_MSG_LENGTH];

      va_list arg;
      va_start(arg, _format);
      vsnprintf(line_out, sizeof (line_out), _format, arg);
      va_end(arg);

      if (use_ts)
         get_timestamp(timestamp, sizeof(timestamp));

      snprintf(ll_msg_out, sizeof (ll_msg_out), "%s%s%s:%s\n",
               use_ts ? timestamp:"", use_ts?":":"", get_log_level_str(loglevel), line_out);

      logging_llprintf(loglevel, "%s", line_out);
      log_terminal_set_message_out(wdgts->msg_out, ll_msg_out);
   }
}
