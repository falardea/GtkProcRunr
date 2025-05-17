/**
 * @brief The implementation
 */
#include <stdio.h>
#include <string.h>
#include "app_widgets.h"
#include "pseudo_app_model/app_interface.h"
#include "parse_args.h"

static void print_usage(char *argv) {
   const char *exeName = strrchr(argv, '/') + 1;  // the +1 removes the leading '/'
   printf("Usage: %s\n"
          "A project for experimenting with GTK3 and design patterns in C.\n"
          " -D --debug\tEnable Debugging and enable Debug (full) level logging\n"
          " -T --trace\tEnable Debugging+ adds a LOGLEVEL_TRACE\n"
          " -q --quiet\tRaise log level to ERROR\n"
          " +t --timestamps\tEnable timestamps\n"
          " -c --console\tSkip the UI, just run main as a console app\n"
          " -h --help\tPrint this help message\n\n", exeName);
}

RVALUE parse_input_args(int argc, char **argv) {
   bool debugging_enabled = false;
   bool trace_flag_enabled = false;
   bool quiet_enabled = false;
   bool timestamps_enabled = false;
   bool console_only_enabled = false;

   for (int i = 1; i < argc; i++) {
      if ((strcmp(argv[i], "-D") == 0) || (strcmp(argv[i], "--debug") == 0))
      {
         debugging_enabled = true;
      } else if ((strcmp(argv[i], "-T") == 0) || (strcmp(argv[i], "--trace") == 0))
      {
         trace_flag_enabled = true;
      } else if ((strcmp(argv[i], "-q") == 0) || (strcmp(argv[i], "--quiet") == 0))
      {
         quiet_enabled = true;
      } else if ((strcmp(argv[i], "+t") == 0) || (strcmp(argv[i], "--timestamps") == 0))
      {
         timestamps_enabled = true;
      } else if ((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--console") == 0))
      {
         console_only_enabled = true;
      } else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
      {
         print_usage(argv[0]);
      } else if (strlen(argv[i]) > 0) {
         print_usage(argv[0]);
         return RVALUE_ERROR;
      }
   }

   set_app_debug_flag(debugging_enabled);

   // The order here is preference in which has override.  I figure if you set debug, you meant it
   // whereas "quiet" would be an automatic inclusion in most builds for production or nearing it
   if (quiet_enabled) set_app_log_level(LOGLEVEL_ERROR);
   if (debugging_enabled) set_app_log_level(LOGLEVEL_DEBUG);
   if (trace_flag_enabled) set_app_log_level(LOGLEVEL_TRACE);

   set_app_log_w_timestamp_flag(timestamps_enabled);
   set_app_run_console_only(console_only_enabled);

   return RVALUE_SUCCESS;
}
