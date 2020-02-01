#include "saving.h"

StxParams default_params() {
  StxParams params;
  params.scale_x = 100;
  params.scale_y = 100;
  params.e6_write = FALSE;
  params.magical_number = 4;
  return params;
}
