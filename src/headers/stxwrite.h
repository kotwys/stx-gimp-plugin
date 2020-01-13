#pragma once
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

StxError write(
  const StxParams &params,
  gint32 drawable_id,
  std::ofstream &file
);
