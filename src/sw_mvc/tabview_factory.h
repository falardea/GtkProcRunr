/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef TABVIEW_FACTORY_H__
#define TABVIEW_FACTORY_H__
#include <gtk/gtk.h>
#include "run_model.h"
#include "run_viewer.h"
#include "setup_view.h"
G_BEGIN_DECLS // __cplusplus guard

typedef enum
{
   SETUP_TAB,
   SERVICE_TAB,

} TABVIEW_FACTORY_ORDERS;

// VIEW_BUILDER
#define TABVIEW_TYPE_FACTORY (tabview_factory_get_type())

G_DECLARE_FINAL_TYPE(TabviewFactory, tabview_factory, TABVIEW, FACTORY, GObject)

TabviewFactory *tabview_factory_new(RunViewer *run_viewer, SetupViewer *setup_viewer, RunModel *run_model);

void tabview_factory_build_setup_viewer(TabviewFactory *self);

G_END_DECLS
#endif  // TABVIEW_FACTORY_H__
