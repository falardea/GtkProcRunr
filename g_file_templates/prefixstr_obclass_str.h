/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef PREFIXSTR_OBCLASS_STR_H__
#define PREFIXSTR_OBCLASS_STR_H__
#include <gtk/gtk.h>
G_BEGIN_DECLS // __cplusplus guard

#define PREFIXSTR_TYPE_OBCLASS_STR (prefixstr_obclass_str_get_type())

G_DECLARE_FINAL_TYPE(PrefixstrObclassStr, prefixstr_obclass_str, PREFIXSTR, OBCLASS_STR, GObject)

PrefixstrObclassStr *prefixstr_obclass_str_new();

G_END_DECLS
#endif  // PREFIXSTR_OBCLASS_STR_H__
