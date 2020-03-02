#include <gtkmm.h>
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "ui/stxsavedialog.h"
#include "ui/stxparams.h"
#include "dialog.h"

bool save_dialog(StxParams &params) {
  auto dialog = stx_save_dialog_new(&params);
  gtk_widget_show_all(dialog);
  auto response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  return response == GTK_RESPONSE_OK;
}
