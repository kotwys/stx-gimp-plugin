#include "ui/stxsavedialog.h"

struct _StxSaveDialog {
  GtkDialog parent_instance;

  GtkWidget *e6_checkbox;
  GtkWidget *scale_x_box;
  GtkWidget *scale_y_box;
  GtkWidget *magic_box;

  StxParams *params;
};

typedef enum {
  PROP_DATA = 1,
  N_PROPERTIES
} StxDialogProperty;

static GtkWidget *number_field(
  const gchar* label_text,
  GtkObject* adjustment
) {
  GtkWidget *hbox = gtk_hbox_new(FALSE, 10);
  GtkWidget *label = gtk_label_new(label_text);
  GtkWidget *spin =
    gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 5, 1);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);
  return hbox;
}

static GtkWidget *helper_get_child(const GtkWidget* container, guint id) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(container));
  GtkWidget *child = GTK_WIDGET(g_list_nth_data(children, id));
  g_list_free(children);
  return child;
}

G_DEFINE_TYPE(StxSaveDialog, stx_save_dialog, GTK_TYPE_DIALOG)

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void stx_save_dialog_set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
) {
  StxSaveDialog *self = STX_SAVE_DIALOG(object);
  GtkDialog parent = self->parent_instance;

  switch ((StxDialogProperty) property_id) {
    case PROP_DATA:
      self->params = g_value_get_pointer(value);
      break;

    default:
      g_object_set_property(
        G_OBJECT(&parent),
        g_param_spec_get_name(pspec),
        value
      );
      break;
  }
}

static void stx_save_dialog_get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
) {
  StxSaveDialog *self = STX_SAVE_DIALOG(object);
  GtkDialog parent = self->parent_instance;

  switch ((StxDialogProperty) property_id) {
    case PROP_DATA:
      // temporary solution
      self->params->scale_x = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(helper_get_child(self->scale_x_box, 1))
      );
      self->params->scale_y = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(helper_get_child(self->scale_y_box, 1))
      );
      self->params->magical_number = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(helper_get_child(self->magic_box, 1))
      );
      self->params->e6_write = gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(self->e6_checkbox)
      );
      g_value_set_pointer(value, self->params);
      break;

    default:
      g_object_get_property(
        G_OBJECT(&parent),
        g_param_spec_get_name(pspec),
        value
      );
      break;
  }
}

static void stx_save_dialog_class_init(StxSaveDialogClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->set_property = stx_save_dialog_set_property;
  object_class->get_property = stx_save_dialog_get_property;

  obj_properties[PROP_DATA] = g_param_spec_pointer(
    "model-data", "Model data",
    "Model data to use",
    G_PARAM_READWRITE
  );

  g_object_class_install_properties(
    object_class, N_PROPERTIES, obj_properties
  );
}

static void stx_save_dialog_init(StxSaveDialog *self) {
  gtk_window_set_title(GTK_WINDOW(self), "Save STX texture");
  gtk_window_set_role(GTK_WINDOW(self), DIALOG_ID);
  gtk_dialog_add_buttons(
    GTK_DIALOG(self),
    "Cancel", GTK_RESPONSE_CANCEL,
    "OK", GTK_RESPONSE_OK,
    NULL
  );

  GtkWidget *vbox = gtk_vbox_new(FALSE, 12);
  gtk_container_set_border_width(GTK_CONTAINER(vbox), 12);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(self)->vbox), vbox);

  self->e6_checkbox =
    gtk_check_button_new_with_label("Write E6 section?");
  gtk_box_pack_start(GTK_BOX(vbox), self->e6_checkbox, FALSE, FALSE, 0);

  GtkObject *scale_x_adjustment = gtk_adjustment_new(
    100, 0, 10000, 1, 10, 0
  );
  self->scale_x_box = number_field("Scale X", scale_x_adjustment);
  gtk_box_pack_start(GTK_BOX(vbox), self->scale_x_box, FALSE, FALSE, 0);

  GtkObject *scale_y_adjustment = gtk_adjustment_new(
    100, 0, 10000, 1, 10, 0
  );
  self->scale_y_box = number_field("Scale Y", scale_y_adjustment);
  gtk_box_pack_start(GTK_BOX(vbox), self->scale_y_box, FALSE, FALSE, 0);
  
  GtkObject *magic_adjustment = gtk_adjustment_new(4, 1, 255, 1, 10, 0);
  self->magic_box = number_field("Magic number", magic_adjustment);
  gtk_box_pack_start(GTK_BOX(vbox), self->magic_box, FALSE, FALSE, 0);
}

GtkWidget *stx_save_dialog_new(StxParams *params) {
  return (GtkWidget*) g_object_new(
    stx_save_dialog_get_type(),
    "model-data", params, NULL
  );
}
