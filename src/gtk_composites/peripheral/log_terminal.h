/**
 * Created by french on 5/3/25.
 * @brief
 */
#ifndef LOG_TERMINAL_H__
#define LOG_TERMINAL_H__
#include <gtk/gtk.h>
G_BEGIN_DECLS // __cplusplus guard

#define LOG_TYPE_TERMINAL (log_terminal_get_type())

G_DECLARE_FINAL_TYPE(LogTerminal, log_terminal, LOG, TERMINAL, GtkScrolledWindow)

LogTerminal *log_terminal_new();

void log_terminal_set_message_out(LogTerminal *self, const char *msgout);

G_END_DECLS
#endif  // LOG_TERMINAL_H__
