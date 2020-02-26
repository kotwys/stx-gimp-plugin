#include "ui/stxparams.h"

StxParams stx_params_default() {
  StxParams params;
  params.scale_x = 100;
  params.scale_y = 100;
  params.e6_write = FALSE;
  params.magical_number = 1024;
  return params;
}
