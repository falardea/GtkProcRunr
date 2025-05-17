/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "tabview_factory.h"
#include "utils/logging.h"
#include "setup_page_templates/command_step.h"
#include "setup_page_templates/uchoice_step.h"
#include "setup_page_templates/branch_step.h"
#include "setup_view.h"

typedef struct
{
   GHashTable *step_map;

} TabviewFactoryPrivate;

struct _TabviewFactory
{
   GObject super;

   SetupViewer *setup_viewer;
   RunModel    *run_model;

   RunViewer   *run_viewer;
};

G_DEFINE_TYPE_WITH_PRIVATE(TabviewFactory, tabview_factory, G_TYPE_OBJECT)

static void tabview_factory_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   TabviewFactory *self = TABVIEW_FACTORY(g_object);
   TabviewFactoryPrivate *priv = tabview_factory_get_instance_private(self);
   if(priv->step_map)
   {
      g_hash_table_destroy(priv->step_map);
   }
   G_OBJECT_CLASS(tabview_factory_parent_class)->finalize(g_object);
}

static void tabview_factory_class_init(TabviewFactoryClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = tabview_factory_finalize;
}

static void tabview_factory_build_step_map(TabviewFactory *self);
static void tabview_factory_init(TabviewFactory *self)
{
   TabviewFactoryPrivate *priv = tabview_factory_get_instance_private(self);
   priv->step_map = g_hash_table_new(NULL, NULL);
}

TabviewFactory *tabview_factory_new(RunViewer *run_viewer, SetupViewer *setup_viewer, RunModel *run_model)
{
   TabviewFactory *self;
   self = g_object_new(TABVIEW_TYPE_FACTORY, NULL);

   self->run_viewer = run_viewer;
   self->setup_viewer = setup_viewer;
   self->run_model = run_model;

   tabview_factory_build_step_map(self);

   return self;
}

static void tabview_factory_build_step_map(TabviewFactory *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   TabviewFactoryPrivate *priv = tabview_factory_get_instance_private(self);

   g_hash_table_insert(priv->step_map, GINT_TO_POINTER(RUN_SETUP_UNINITIALIZED),
                       command_step_new("This is not suppose to be shown", "Goto Mode Select", self->run_model, RUN_SETUP_MODE_SELECTED));

   g_hash_table_insert(priv->step_map, GINT_TO_POINTER(RUN_SETUP_MODE_SELECTED),
                       command_step_new("Welcome to POD (run mode selected)", "Start", self->run_model, RUN_SETUP_MEMCHECK_COMPLETE));

   g_hash_table_insert(priv->step_map, GINT_TO_POINTER(RUN_SETUP_MEMCHECK_COMPLETE),
                       uchoice_step_new("Memcheck complete", "Retry", "Next", self->run_model, RUN_SETUP_MODE_SELECTED, RUN_SETUP_INTERMEDIATE_STEPS));

   g_hash_table_insert(priv->step_map, GINT_TO_POINTER(RUN_SETUP_INTERMEDIATE_STEPS),
                       branch_step_new("A wait step", self->run_model, RUN_MODEL_SETUP_LEAK_COMPLETE_PROP_STR, RUN_SETUP_COMPLETE));

   g_hash_table_insert(priv->step_map, GINT_TO_POINTER(RUN_SETUP_COMPLETE),
                       command_step_new("Setup Complete", "Done", self->run_model, RUN_SETUP_FAILED));

   g_hash_table_insert(priv->step_map, GINT_TO_POINTER(RUN_SETUP_FAILED),
                       command_step_new("A Failure has occurred", "Exit", self->run_model, RUN_SETUP_FAILED));
}

static void per_map_fn(gpointer key, gpointer value, gpointer user_data)
{
   TabviewFactory *self = TABVIEW_FACTORY(user_data);
   setup_viewer_add_page_by_state_id(self->setup_viewer, GTK_WIDGET(value), GPOINTER_TO_INT(key));
}

void tabview_factory_build_setup_viewer(TabviewFactory *self)
{
   TabviewFactoryPrivate *priv = tabview_factory_get_instance_private(self);
   g_hash_table_foreach(priv->step_map, per_map_fn, self);
   setup_viewer_set_page_view(self->setup_viewer, RUN_SETUP_UNINITIALIZED);
}


