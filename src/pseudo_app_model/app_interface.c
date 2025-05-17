/**
 * @brief The implementation
 */
#include "app_interface.h"
#include "utils/sys_interface.h"
#include "utils/parse_args.h"
#include "utils/logging.h"
#include "views/ui_builder.h"
#include "views/css_styler.h"

const LOGLEVEL DEFAULT_APP_LOG_LEVEL = LOGLEVEL_INFO;

static app_model_t sAppModel = {APP_STATE_UNINITIALIZED,
                                false,
                                false,
                                false,
                                DEFAULT_APP_LOG_LEVEL};

const char *APP_STATE_UNINITIALIZED_STR = "APP_STATE_UNINITIALIZED";
const char *APP_STATE_SUCCESS_STR = "APP_STATE_SUCCESS";
const char *APP_STATE_ERROR_STR = "APP_STATE_ERROR";

app_widgets *g_app_widgets;

void app_init(int argc, char **argv) {
   RVALUE retVal;

   set_app_state(APP_STATE_UNINITIALIZED);

   retVal = parse_input_args(argc, argv);
   if (retVal == RVALUE_ERROR) {
      set_app_state(APP_STATE_ERROR);
      return;
   }

   retVal = set_display_env();
   if (retVal == RVALUE_ERROR) {
      set_app_state(APP_STATE_ERROR);
      return;
   }

   if (!get_app_run_console_only())
   {
      gtk_init(&argc, &argv);

      g_app_widgets = app_builder();

      apply_app_styling(g_app_widgets);
   }

   set_app_state(APP_STATE_SUCCESS);
}

void app_finalize(void)
{
   logging_llprintf(LOGLEVEL_INFO, "Goodbye!");
}

const char *get_state_str(APP_INTERFACE_STATE state)
{
   if (state == APP_STATE_UNINITIALIZED)
      return APP_STATE_UNINITIALIZED_STR;
   else if (state == APP_STATE_SUCCESS)
      return APP_STATE_SUCCESS_STR;
   else return APP_STATE_ERROR_STR;
}

void set_app_state(APP_INTERFACE_STATE state) {
   sAppModel.app_state = state;
}
APP_INTERFACE_STATE get_app_state(void) {
   return sAppModel.app_state;
}

void set_app_debug_flag(bool enable) {
   sAppModel.debugging_enabled = enable;
   if (enable) {
      logging_llprintf(LOGLEVEL_INFO, "debug log level enabled");
   }
}

void set_app_log_w_timestamp_flag(bool enable) {
   sAppModel.log_with_timestamps = enable;
}
bool get_app_log_w_timestamp_flag(void) {
   return sAppModel.log_with_timestamps;
}

void set_app_log_level(LOGLEVEL logLevel) {
   if (sAppModel.runtime_log_level > logLevel) {
      sAppModel.runtime_log_level = logLevel;
   }
}
LOGLEVEL get_app_log_level(void) {
   return sAppModel.runtime_log_level;
}

void set_app_run_console_only(bool console_only)
{
   sAppModel.run_console_only = console_only;
}
bool get_app_run_console_only(void)
{
   return sAppModel.run_console_only;
}