/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "run_viewer.h"
#include "utils/logging.h"

#include "setup_view.h"
#include "gtk_composites/service_context.h"
#include "gtk_composites/standard_context.h"
#include "tabview_factory.h"

typedef struct {
   TabviewFactory  *tab_factory; // GObject, disposal?

   StandardContext   *standard_ctx;
   ServiceContext    *service_ctx;
   SetupViewer       *setup_viewer;
}RunViewerPrivate;

struct _RunViewer
{
   GtkBox         super;
   GtkBox         *box_run_view_root;

   GtkBox         *sensors_viewer;
   GtkNotebook    *run_interface;

   GtkLabel       *tablbl_setup_interface;
   GtkLabel       *tablbl_running_interface;
   GtkLabel       *tablbl_misc_interfaces;

   GtkBox         *box_setup_interface;
   GtkBox         *box_running_interface;
   GtkBox         *box_misc_interface;

   RunModel       *model;
};

G_DEFINE_TYPE_WITH_PRIVATE(RunViewer, run_viewer, GTK_TYPE_BOX)

static void run_viewer_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   RunViewer *self = RUN_VIEWER(g_object);
   RunViewerPrivate  *priv = run_viewer_get_instance_private(self);
   if(priv->tab_factory)
   {
      g_object_unref(G_OBJECT(priv->tab_factory));
   }
   G_OBJECT_CLASS(run_viewer_parent_class)->finalize(g_object);
}

static void run_viewer_class_init(RunViewerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = run_viewer_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/run_viewer");
   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_run_view_root);

   gtk_widget_class_bind_template_child(widget_class, RunViewer, sensors_viewer);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, run_interface);

   gtk_widget_class_bind_template_child(widget_class, RunViewer, tablbl_setup_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, tablbl_running_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, tablbl_misc_interfaces);

   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_setup_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_running_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_misc_interface);
}

static void run_viewer_init(RunViewer *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

gboolean run_viewer_mode_change_listener(RunModel *model, RUN_MODEL_MODE mode, gpointer user_data);

RunViewer *run_viewer_new(RunModel *model)
{
   RunViewer *self;
   self = g_object_new(RUN_TYPE_VIEWER, NULL);
   self->model = model;

   RunViewerPrivate *priv = run_viewer_get_instance_private(self);

   priv->standard_ctx = standard_context_new(self->model);
   priv->service_ctx = service_context_new(self->model);
   priv->setup_viewer = setup_viewer_new(self->model);

   priv->tab_factory = tabview_factory_new(self, priv->setup_viewer, self->model);
   tabview_factory_build_setup_viewer(priv->tab_factory);

   gtk_box_pack_start(self->box_setup_interface, GTK_WIDGET(priv->setup_viewer), TRUE, TRUE, 0);
   gtk_box_pack_start(self->box_setup_interface, GTK_WIDGET(priv->standard_ctx), TRUE, TRUE, 0);
   gtk_box_pack_start(self->box_setup_interface, GTK_WIDGET(priv->service_ctx), TRUE, TRUE, 0);

   gtk_widget_set_visible(GTK_WIDGET(priv->setup_viewer), TRUE);
   gtk_widget_set_visible(GTK_WIDGET(priv->standard_ctx), FALSE);
   gtk_widget_set_visible(GTK_WIDGET(priv->service_ctx), FALSE);

   g_signal_connect (G_OBJECT(model), RUN_MODEL_MODE_CHANGE_SIGNAL_STR, G_CALLBACK(run_viewer_mode_change_listener), self);

   return self;
}

gboolean run_viewer_mode_change_listener(RunModel *model, RUN_MODEL_MODE mode, gpointer user_data)
{
   // This could be a broadcast to all children in the view to update based on a model change
   RunViewer *self = RUN_VIEWER(user_data);

   RunViewerPrivate *priv = run_viewer_get_instance_private(self);
   if (run_model_get_run_mode(model) ==  RUN_MODE_STANDARD)
   {
      if (run_model_get_step(model) != RUN_SETUP_COMPLETE)
      {
         gtk_widget_set_visible(GTK_WIDGET(priv->setup_viewer), TRUE);
         gtk_widget_set_visible(GTK_WIDGET(priv->standard_ctx), FALSE);
         gtk_widget_set_visible(GTK_WIDGET(priv->service_ctx), FALSE);
      }
      else
      {
         gtk_widget_set_visible(GTK_WIDGET(priv->setup_viewer), FALSE);
         gtk_widget_set_visible(GTK_WIDGET(priv->standard_ctx), TRUE);
         gtk_widget_set_visible(GTK_WIDGET(priv->service_ctx), FALSE);
      }
   }
   else
   {
      gtk_widget_set_visible(GTK_WIDGET(priv->setup_viewer), FALSE);
      gtk_widget_set_visible(GTK_WIDGET(priv->standard_ctx), FALSE);
      gtk_widget_set_visible(GTK_WIDGET(priv->service_ctx), TRUE);
   }
   run_model_set_step(self->model, RUN_SETUP_MODE_SELECTED);
   return G_SOURCE_REMOVE;
}
