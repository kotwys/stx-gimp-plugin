#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "dialog.h"

static GtkWidget *number_field(
  const char* label_text,
  GtkObject* adjustment
) {
  GtkWidget* hbox = gtk_hbox_new(FALSE, 10);
  auto label = gtk_label_new(label_text);
  auto spin = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 5, 1);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);
  return hbox;
}

GtkWidget *get_child(const GtkWidget* container, guint id) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(container));
  GtkWidget *child = GTK_WIDGET(g_list_nth_data(children, id));
  g_list_free(children);
  return child;
}

bool save_dialog(StxParams &params) {
  GtkWidget *dialog = gimp_dialog_new(
    "Save STX texture", DIALOG_ID,
    NULL, 0,
    NULL, 0,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OK, GTK_RESPONSE_OK,
    NULL
  );

  auto vbox = gtk_vbox_new(FALSE, 12);
  gtk_container_set_border_width(GTK_CONTAINER(vbox), 12);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), vbox);

  auto e6_checkbox = gtk_check_button_new_with_label("Write E6 section?");
  gtk_box_pack_start(GTK_BOX(vbox), e6_checkbox, FALSE, FALSE, 0);

  auto scale_x_adjustment = gtk_adjustment_new(
    100, 0, 10000, 1, 10, 0
  );
  auto scale_x_box = number_field("Scale X", scale_x_adjustment);
  gtk_box_pack_start(GTK_BOX(vbox), scale_x_box, FALSE, FALSE, 0);

  auto *scale_y_adjustment = gtk_adjustment_new(
    100, 0, 10000, 1, 10, 0
  );
  auto scale_y_box = number_field("Scale Y", scale_y_adjustment);
  gtk_box_pack_start(GTK_BOX(vbox), scale_y_box, FALSE, FALSE, 0);
  
  auto *magic_adjustment = gtk_adjustment_new(4, 1, 255, 1, 10, 0);
  auto magic_box = number_field("Magic number", magic_adjustment);
  gtk_box_pack_start(GTK_BOX(vbox), magic_box, FALSE, FALSE, 0);

  gtk_widget_show_all(dialog);
  auto response = gimp_dialog_run(GIMP_DIALOG(dialog));

  if (response == GTK_RESPONSE_OK) {
    // Spin button is the second child of the field.
    params.scale_x = gtk_spin_button_get_value_as_int(
      GTK_SPIN_BUTTON(get_child(scale_x_box, 1))
    );
    params.scale_y = gtk_spin_button_get_value_as_int(
      GTK_SPIN_BUTTON(get_child(scale_y_box, 1))
    );
    params.magical_number = gtk_spin_button_get_value_as_int(
      GTK_SPIN_BUTTON(get_child(magic_box, 1))
    );
    params.e6_write = gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(e6_checkbox)
    );
  }

  gtk_widget_destroy(dialog);
  
  return response == GTK_RESPONSE_OK;
}
