#pragma once
#include <variant>
#include <glib.h>
#include <iostream>

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
  std::ostream &file
);
StxResult<std::monostate> write(
  const StxParams &params,
  gint32 drawable_id,
  std::ostream &file
);
