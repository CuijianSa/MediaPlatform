#include "util_log.h"


int main(int argc, char *argv[]) {
  LOG_SET_LEVEL(LOG_LEVEL_VERBOSE);
  LOG_VERBOSE("LOG_EXAMPLE", "THis is %s log", "verbose");
  LOG_DEBUG("LOG_EXAMPLE", "THis is %s log", "debug");
  LOG_INFO("LOG_EXAMPLE", "THis is %s log", "info");
  LOG_WARN("LOG_EXAMPLE", "THis is %s log", "warn");
  LOG_ERROR("LOG_EXAMPLE", "THis is %s log", "error");
  LOG_ASSERT("LOG_EXAMPLE", "THis is %s log", "assert");
}