#include "ui/stxsavedialog.h"
#include "ui/stxnumberfield.h"

struct _StxSaveDialog {
  GtkDialog parent_instance;

  GtkWidget *e6_checkbox;
  GtkWidget *scale_x_box;
  GtkWidget *scale_y_box;
  GtkWidget *magic_box;

  StxConfig *config;
};

typedef enum {
  PROP_CONFIG = 1,
  N_PROPERTIES
} StxDialogProperty;

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
    case PROP_CONFIG:
      self->config = g_value_get_pointer(value);
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
    case PROP_CONFIG:
      g_value_set_pointer(value, self->config);
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

  obj_properties[PROP_CONFIG] = g_param_spec_pointer(
    "config", "Saving config",
    "Configuration for saving.",
    G_PARAM_READWRITE
  );

  g_object_class_install_properties(
    object_class, N_PROPERTIES, obj_properties
  );
}

static void update_e6_write(
  GtkToggleButton *button,
  StxSaveDialog *self
) {
  self->config->e6_write = gtk_toggle_button_get_active(button);
}

static void update_scale_x(
  StxNumberField *field,
  StxSaveDialog *self
) {
  self->config->scale_x =
    (guint16) stx_number_field_get_value_as_int(field);
}

static void update_scale_y(
  StxNumberField *field,
  StxSaveDialog *self
) {
  self->config->scale_y =
    (guint16) stx_number_field_get_value_as_int(field);
}

static void update_magic(
  StxNumberField *field,
  StxSaveDialog *self
) {
  self->config->magical_number =
    (guint16) stx_number_field_get_value_as_int(field);
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
  g_signal_connect(
    self->e6_checkbox, "toggled",
    (GCallback) update_e6_write, self
  );
  gtk_box_pack_start(GTK_BOX(vbox), self->e6_checkbox, FALSE, FALSE, 0);

  GtkObject *scale_x_adjustment = gtk_adjustment_new(
    100, 0, 10000, 1, 10, 0
  );
  self->scale_x_box = stx_number_field_new(
    "Scale X",
    GTK_ADJUSTMENT(scale_x_adjustment)
  );
  g_signal_connect(
    self->scale_x_box, "value-changed",
    (GCallback) update_scale_x, self
  );
  gtk_box_pack_start(GTK_BOX(vbox), self->scale_x_box, FALSE, FALSE, 0);

  GtkObject *scale_y_adjustment = gtk_adjustment_new(
    100, 0, 10000, 1, 10, 0
  );
  self->scale_y_box = stx_number_field_new(
    "Scale Y",
    GTK_ADJUSTMENT(scale_y_adjustment)
  );
  g_signal_connect(
    self->scale_y_box, "value-changed",
    (GCallback) update_scale_y, self
  );
  gtk_box_pack_start(GTK_BOX(vbox), self->scale_y_box, FALSE, FALSE, 0);
  
  GtkObject *magic_adjustment = gtk_adjustment_new(1024, 1, 65535, 1, 10, 0);
  self->magic_box = stx_number_field_new(
    "Magic number",
    GTK_ADJUSTMENT(magic_adjustment)
  );
  g_signal_connect(
    self->magic_box, "value-changed",
    (GCallback) update_magic, self
  );
  gtk_box_pack_start(GTK_BOX(vbox), self->magic_box, FALSE, FALSE, 0);
}

GtkWidget *stx_save_dialog_new(StxConfig *config) {
  return (GtkWidget*) g_object_new(
    stx_save_dialog_get_type(),
    "config", config, NULL
  );
}
