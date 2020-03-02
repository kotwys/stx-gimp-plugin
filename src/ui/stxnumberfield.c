#include "ui/stxnumberfield.h"

typedef enum {
  PROP_LABEL = 1,
  PROP_ADJUSTMENT,
  PROP_VALUE,
  N_PROPERTIES
} NumberFieldProps;

enum {
  VALUE_CHANGED,
  N_SIGNALS
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };
static guint obj_signals[N_SIGNALS] = { 0, };

struct _StxNumberField {
  GtkHBox parent_instance;

  GtkWidget *label;
  GtkWidget *spin;
};

G_DEFINE_TYPE(StxNumberField, stx_number_field, GTK_TYPE_HBOX)

static void stx_number_field_set_property(
  GObject *object,
  guint property_id,
  const GValue *value,
  GParamSpec *pspec
) {
  StxNumberField *self = STX_NUMBER_FIELD(object);

  switch ((NumberFieldProps) property_id) {
    case PROP_LABEL:
      g_object_set_property(G_OBJECT(self->label), "label", value);
      break;

    case PROP_ADJUSTMENT:
      g_object_set_property(G_OBJECT(self->spin), "adjustment", value);
      break;

    case PROP_VALUE:
      g_object_set_property(G_OBJECT(self->spin), "value", value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static void stx_number_field_get_property(
  GObject *object,
  guint property_id,
  GValue *value,
  GParamSpec *pspec
) {
  StxNumberField *self = STX_NUMBER_FIELD(object);

  switch ((NumberFieldProps) property_id) {
    case PROP_LABEL:
      g_object_get_property(G_OBJECT(self->label), "label", value);
      break;

    case PROP_ADJUSTMENT:
      g_object_get_property(G_OBJECT(self->spin), "adjustment", value);
      break;

    case PROP_VALUE:
      g_object_get_property(G_OBJECT(self->spin), "value", value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static void stx_number_field_class_init(StxNumberFieldClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->set_property = stx_number_field_set_property;
  object_class->get_property = stx_number_field_get_property;

  obj_properties[PROP_LABEL] = g_param_spec_string(
    "label", "Label",
    "Label for the field.",
    NULL, G_PARAM_READWRITE
  );

  obj_properties[PROP_ADJUSTMENT] = g_param_spec_object(
    "adjustment", "Adjustment",
    "Adjustment for the spin button.",
    GTK_TYPE_ADJUSTMENT, G_PARAM_READWRITE
  );

  obj_properties[PROP_VALUE] = g_param_spec_double(
    "value", "Value",
    "Current value.",
    -G_MAXDOUBLE, G_MAXDOUBLE,
    0.0, G_PARAM_READWRITE
  );

  g_object_class_install_properties(
    object_class, N_PROPERTIES, obj_properties
  );

  obj_signals[VALUE_CHANGED] = g_signal_new(
    "value-changed",
    G_TYPE_FROM_CLASS(object_class),
    G_SIGNAL_RUN_LAST,
    0, NULL, NULL, NULL,
    G_TYPE_NONE, 0
  );
}

static void stx_number_field_on_value_changed(
  GtkSpinButton *spin,
  StxNumberField *self
) {
  g_signal_emit(self, obj_signals[VALUE_CHANGED], 0, self);
}

static void stx_number_field_init(StxNumberField *self) {
  gtk_box_set_spacing(GTK_BOX(self), 10);
  
  self->label = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(self), self->label, FALSE, FALSE, 0);

  self->spin = gtk_spin_button_new_with_range(G_MINDOUBLE, G_MAXDOUBLE, 1);
  g_signal_connect(
    self->spin, "value-changed",
    (GCallback) stx_number_field_on_value_changed,
    self
  );
  gtk_box_pack_start(GTK_BOX(self), self->spin, FALSE, FALSE, 0);
}

GtkWidget *stx_number_field_new(
  const gchar *label,
  GtkAdjustment *adjustment
) {
  return (GtkWidget*) g_object_new(
    stx_number_field_get_type(),
    "label", label,
    "adjustment", adjustment,
    NULL
  );
}

gint stx_number_field_get_value_as_int(StxNumberField *field) {
  gdouble value;
  g_object_get(G_OBJECT(field), "value", &value, NULL);
  return (gint) value;
}
