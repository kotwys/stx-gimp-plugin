#pragma once

#include <fstream>
#include "structure.h"
#include "value.h"

StxError to_image(gint32 &image_id);

class stxread {
private:
  const char *filename;
  std::ifstream file;
  Geometry geometry;
  guchar* image_data;

public:
  stxread(const char *filename);
  ~stxread();

  bool good() const;

  StxError process();
  StxError to_image(gint32 &image_id);
};
