/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "service_context.h"
#include "utils/logging.h"

struct _ServiceContext
{
   GtkBox super;
   GtkBox *root_content;

   RunModel *model;
};

G_DEFINE_TYPE(ServiceContext, service_context, GTK_TYPE_BOX)

static void service_context_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS(service_context_parent_class)->finalize(g_object);
}

static void service_context_class_init(ServiceContextClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = service_context_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/service_context");
   gtk_widget_class_bind_template_child(widget_class, ServiceContext, root_content);

}

static void service_context_init(ServiceContext *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

ServiceContext *service_context_new(RunModel *model)
{
   ServiceContext *self;
   self = g_object_new(SERVICE_TYPE_CONTEXT, NULL);
   self->model = model;
   return self;
}
