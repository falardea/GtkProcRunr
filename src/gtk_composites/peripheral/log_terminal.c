/**
 * Created by french on 5/3/25.
 * @brief
 */
#include "log_terminal.h"
#include "utils/logging.h"

const char *MSG_OUT_CURSOR_NAME = "msgOutCursor";

struct _LogTerminal
{
   GtkScrolledWindow             super;
   GtkTextView                   *message_out;
   GtkTextMark                   *msgOutCursor;
};

G_DEFINE_TYPE(LogTerminal, log_terminal, GTK_TYPE_SCROLLED_WINDOW)

static void log_terminal_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS(log_terminal_parent_class)->finalize(g_object);
}

static void log_terminal_class_init(LogTerminalClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = log_terminal_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/log_terminal");
   gtk_widget_class_bind_template_child(widget_class, LogTerminal, message_out);
}

static void log_terminal_init(LogTerminal *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
   self->msgOutCursor = NULL;
}

LogTerminal *log_terminal_new()
{
   LogTerminal *self;
   self = g_object_new(LOG_TYPE_TERMINAL, NULL);

   return self;
}

void log_terminal_set_message_out(LogTerminal *self, const char *msgout)
{
   GtkTextBuffer  *tvBuff  =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->message_out));
   GtkTextIter    endIter;
   gtk_text_buffer_get_end_iter(tvBuff, &endIter);

   if (!gtk_text_buffer_get_mark(tvBuff, MSG_OUT_CURSOR_NAME))
   {
      self->msgOutCursor = gtk_text_mark_new(MSG_OUT_CURSOR_NAME, FALSE);
      gtk_text_buffer_add_mark(tvBuff, self->msgOutCursor, &endIter);
   }

   gtk_text_buffer_insert(tvBuff, &endIter, msgout, -1);

   // This keeps the latest msgout in view, BUT if you were scrolling up and a new msgout was posted, it will
   // autoscroll to the insertion.  It would be better to only auto-scroll the msgout if the user is not looking
   // at previous messages
   gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(self->message_out), self->msgOutCursor, 0.0, TRUE, 0.0, 0.0);
}