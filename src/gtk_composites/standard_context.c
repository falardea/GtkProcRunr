/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "standard_context.h"
#include "utils/logging.h"

struct _StandardContext
{
   GtkBox super;
   GtkBox *root_content;

   RunModel *model;
};

G_DEFINE_TYPE(StandardContext, standard_context, GTK_TYPE_BOX)

static void standard_context_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS(standard_context_parent_class)->finalize(g_object);
}

static void standard_context_class_init(StandardContextClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = standard_context_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/standard_context");
   gtk_widget_class_bind_template_child(widget_class, StandardContext, root_content);
}

static void standard_context_init(StandardContext *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

StandardContext *standard_context_new(RunModel *model)
{
   StandardContext *self;
   self = g_object_new(STANDARD_TYPE_CONTEXT, NULL);
   self->model = model;

   return self;
}
