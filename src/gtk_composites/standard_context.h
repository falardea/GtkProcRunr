/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef STANDARD_CONTEXT_H__
#define STANDARD_CONTEXT_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define STANDARD_TYPE_CONTEXT (standard_context_get_type())

G_DECLARE_FINAL_TYPE(StandardContext, standard_context, STANDARD, CONTEXT, GtkBox)

StandardContext *standard_context_new(RunModel *model);

G_END_DECLS
#endif  // STANDARD_CONTEXT_H__
