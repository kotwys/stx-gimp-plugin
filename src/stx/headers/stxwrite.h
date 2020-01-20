#pragma once
#include <variant>
#include <glib.h>
#include <fstream>

#include "image.h"
#include "value.h"

struct StxParams {
  guint16 scale_x;
  guint16 scale_y;
  gboolean e6_write;
  guint8 magical_number;
};

StxParams default_params();

StxResult<std::monostate> write(
  const StxImage &img,
  std::ofstream &file
);
StxResult<std::monostate> write(
  const StxParams &params,
  gint32 drawable_id,
  std::ofstream &file
);
