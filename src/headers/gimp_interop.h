#pragma once

#include <libgimp/gimp.h>
#include "image.h"
#include "saving.h"
#include "value.h"

StxResult<gint32> to_gimp(const StxImage &data);
StxResult<StxImage> from_gimp(
  const StxParams &params,
  const gint32 drawable_id
);
