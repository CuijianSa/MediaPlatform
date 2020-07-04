#ifndef UTIL_LOG_H_
#define UTIL_LOG_H_

#include <iostream>
#include <memory>

enum LogLevel {
  LOG_LEVEL_VERBOSE = 0,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_ASSERT
};

class UtilLog {
 public:
  static std::shared_ptr<UtilLog> GetInstance();
  UtilLog(enum LogLevel level = LOG_LEVEL_INFO);
  void SetLevel(enum LogLevel level);
  void Print(std::string tag,
             enum LogLevel level,
             const char* file,
             const char* func,
             int l,
             const char* fmt,
             ...);

 protected:
  void PrintColorByLevel(enum LogLevel level);
  void PrintResetColor();
  std::string GetLogLevelName(enum LogLevel level);
 private:
  enum LogLevel level_;
};

#define LOG_SET_LEVEL(level) UtilLog::GetInstance()->SetLevel(level)
#define LOG_VERBOSE(TAG, ...)                                     \
  UtilLog::GetInstance()->Print(TAG, LOG_LEVEL_VERBOSE, __FILE__, \
                                __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(TAG, ...)                                                   \
  UtilLog::GetInstance()->Print(TAG, LOG_LEVEL_DEBUG, __FILE__, __FUNCTION__, \
                                __LINE__, __VA_ARGS__)
#define LOG_INFO(TAG, ...)                                                   \
  UtilLog::GetInstance()->Print(TAG, LOG_LEVEL_INFO, __FILE__, __FUNCTION__, \
                                __LINE__, __VA_ARGS__)
#define LOG_WARN(TAG, ...)                                                   \
  UtilLog::GetInstance()->Print(TAG, LOG_LEVEL_WARN, __FILE__, __FUNCTION__, \
                                __LINE__, __VA_ARGS__)
#define LOG_ERROR(TAG, ...)                                                   \
  UtilLog::GetInstance()->Print(TAG, LOG_LEVEL_ERROR, __FILE__, __FUNCTION__, \
                                __LINE__, __VA_ARGS__)
#define LOG_ASSERT(TAG, ...)                                                   \
  UtilLog::GetInstance()->Print(TAG, LOG_LEVEL_ASSERT, __FILE__, __FUNCTION__, \
                                __LINE__, __VA_ARGS__)

#endif