/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef SERVICE_CONTEXT_H__
#define SERVICE_CONTEXT_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define SERVICE_TYPE_CONTEXT (service_context_get_type())

G_DECLARE_FINAL_TYPE(ServiceContext, service_context, SERVICE, CONTEXT, GtkBox)

ServiceContext *service_context_new(RunModel *model);

G_END_DECLS
#endif  // SERVICE_CONTEXT_H__
