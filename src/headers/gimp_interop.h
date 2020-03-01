#pragma once

#include <libgimp/gimp.h>
#include "stx/image.h"
#include "stx/value.h"
#include "ui/stxparams.h"

stx::Result<gint32> to_gimp(const stx::Image &data);
stx::Result<stx::Image> from_gimp(
  const StxParams &params,
  const gint32 drawable_id
);
