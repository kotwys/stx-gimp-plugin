#pragma once
#include <variant>
#include <glib.h>
#include <fstream>
#include "value.h"

struct StxParams {
  guint16 scale_x;
  guint16 scale_y;
  gboolean e6_write;
  guint8 magical_number;
};

StxParams default_params();

StxResult<std::monostate> write_either(
  const StxParams &params,
  gint32 drawable_id,
  std::ofstream &file
);
[[deprecated("Replaced by write_either")]]
StxError write(
  const StxParams &params,
  gint32 drawable_id,
  std::ofstream &file
);
