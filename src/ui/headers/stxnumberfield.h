#ifndef __STX_NUMBER_FIELD_H__
#define __STX_NUMBER_FIELD_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STX_TYPE_NUMBER_FIELD stx_number_field_get_type()
#define STX_NUMBER_FIELD(obj) \
  GTK_CHECK_CAST(obj, stx_number_field_get_type(), StxNumberField)
#define STX_IS_NUMBER_FIELD(obj) \
  GTK_CHECK_TYPE(obj, stx_number_field_get_type())

typedef struct _StxNumberField StxNumberField;
typedef struct _StxNumberFieldClass StxNumberFieldClass;

struct _StxNumberFieldClass {
  GtkHBoxClass parent_class;
};

GType stx_number_field_get_type(void);
GtkWidget *stx_number_field_new(
  const gchar *label,
  GtkAdjustment *adjustment
);

G_END_DECLS

#endif
