/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "prefixstr_obclass_str.h"

struct _PrefixstrObclassStr
{
   GObject                       super;
};

G_DEFINE_TYPE(PrefixstrObclassStr, prefixstr_obclass_str, G_TYPE_OBJECT)

static void prefixstr_obclass_str_finalize(GObject *g_object)
{
   G_OBJECT_CLASS(prefixstr_obclass_str_parent_class)->finalize(g_object);
}

static void prefixstr_obclass_str_class_init(PrefixstrObclassStrClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = prefixstr_obclass_str_finalize;
}

static void prefixstr_obclass_str_init(PrefixstrObclassStr *self)
{
}

PrefixstrObclassStr *prefixstr_obclass_str_new()
{
   PrefixstrObclassStr *self;
   self = g_object_new(PREFIXSTR_TYPE_OBCLASS_STR, NULL);
   return self;
}
