#include "util_log.h"

#include <stdarg.h>
#include <string.h>

#include <mutex>

#include "util_log_config.h"

static std::shared_ptr<UtilLog> g_util_log = nullptr;
static std::mutex g_mutex;

UtilLog::UtilLog(enum LogLevel level) : level_(level) {}

std::shared_ptr<UtilLog> UtilLog::GetInstance() {
  if (nullptr == g_util_log) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (nullptr == g_util_log) {
      g_util_log = std::make_shared<UtilLog>();
    }
  }
  return g_util_log;
}

void UtilLog::SetLevel(enum LogLevel level) {
  level_ = level;
}

std::string UtilLog::GetLogLevelName(enum LogLevel level) {
  std::string level_name = "unknow";
  switch (level) {
    case LOG_LEVEL_VERBOSE:
      level_name = "verbose";
      break;
    case LOG_LEVEL_DEBUG:
      level_name = "debug";
      break;
    case LOG_LEVEL_INFO:
      level_name = "info";
      break;
    case LOG_LEVEL_WARN:
      level_name = "warn";
      break;
    case LOG_LEVEL_ERROR:
      level_name = "error";
      break;
    case LOG_LEVEL_ASSERT:
      level_name = "assert";
      break;
    default:
      break;
  }
  return level_name;
}

void UtilLog::PrintColorByLevel(enum LogLevel level) {
  switch (level) {
    case LOG_LEVEL_VERBOSE:
      std::cout << GREEN;
      break;
    case LOG_LEVEL_DEBUG:
      std::cout << GREEN;
      break;
    case LOG_LEVEL_INFO:
      std::cout << CYAN;
      break;
    case LOG_LEVEL_WARN:
      std::cout << YELLOW;
      break;
    case LOG_LEVEL_ERROR:
      std::cout << RED;
      break;
    case LOG_LEVEL_ASSERT:
      std::cout << RED;
      break;
    default:
      break;
  }
}

void UtilLog::PrintResetColor() {
  std::cout << RESET;
}

void UtilLog::Print(std::string tag,
                    enum LogLevel level,
                    const char* file,
                    const char* func,
                    int l,
                    const char* fmt,
                    ...) {
  if (level < level_) {
    return;
  }
  va_list ap;
  va_start(ap, fmt);
  PrintColorByLevel(level);
  std::cout << "[" << tag << "] " << GetLogLevelName(level) << " ";
  PrintResetColor();
  const char* r_delimiter = strrchr(file, '/');
  if (r_delimiter) {
    r_delimiter = r_delimiter + 1;
  } else {
    r_delimiter = file;
  }
  std::cout << "[file:" << r_delimiter << "::" << func << "line:"<< l << "]\t";
  vfprintf(stdout, fmt, ap);
  va_end(ap);
  std::cout << std::endl;
}