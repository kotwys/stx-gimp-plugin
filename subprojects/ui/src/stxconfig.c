#include "stxconfig.h"

StxConfig stx_config_default() {
  StxConfig config;
  config.scale_x = 100;
  config.scale_y = 100;
  config.e6_write = FALSE;
  config.magical_number = 1024;
  return config;
}
