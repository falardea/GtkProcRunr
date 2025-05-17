/**
 * @brief Application entry point
 */
#include <gtk/gtk.h>
#include "version.h"
#include "pseudo_app_model/app_interface.h"
#include "headless_sandbox/headless_sandbox.h"
#include "utils/logging.h"

int main(int argc, char **argv) {
   printf("ver.%d.%d.%d\n", project_VERSION_MAJOR, project_VERSION_MINOR, project_VERSION_PATCH);

   app_init(argc, argv);

   if (get_app_state() == APP_STATE_SUCCESS)
   {
      if (!get_app_run_console_only())
      {
         logging_llprintf(LOGLEVEL_INFO, "application initialized, starting UI");
         gtk_main();
         logging_llprintf(LOGLEVEL_INFO, "UI closed, shutting down");
      } else {
         run_console_context();
      }
   }
   else
   {
      logging_llprintf(LOGLEVEL_ERROR, "application initialization failed, attempting a graceful shutdown");
      // Graceful shutdown
   }

   app_finalize();

   return get_app_state();
}
