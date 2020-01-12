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

class stxwrite {
private:
  std::ofstream file;
  const StxParams &params;

public:
  stxwrite(const char *filename, const StxParams &params);
  ~stxwrite();

  bool good() const;
  StxError write(gint32 drawable_id);
};

StxParams default_params();
