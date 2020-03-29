#ifndef __STX_DIALOG_H__
#define __STX_DIALOG_H__

#include <gtk/gtk.h>
#include "stxconfig.h"

#define DIALOG_ID "file-stx"

G_BEGIN_DECLS

#define STX_TYPE_SAVE_DIALOG stx_save_dialog_get_type()
#define STX_SAVE_DIALOG(obj) \
  GTK_CHECK_CAST(obj, stx_save_dialog_get_type(), StxSaveDialog)
#define STX_IS_SAVE_DIALOG(obj) \
  GTK_CHECK_TYPE(obj, stx_save_dialog_get_type())

typedef struct _StxSaveDialog StxSaveDialog;
typedef struct _StxSaveDialogClass StxSaveDialogClass;

struct _StxSaveDialogClass {
  GtkDialogClass parent_class;
};

GType stx_save_dialog_get_type(void);
GtkWidget *stx_save_dialog_new(StxConfig *config);

G_END_DECLS

#endif
