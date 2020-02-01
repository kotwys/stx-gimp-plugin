#pragma once

#include <glib.h>

struct StxParams {
  guint16 scale_x;
  guint16 scale_y;
  gboolean e6_write;
  guint8 magical_number;
};

StxParams default_params();
