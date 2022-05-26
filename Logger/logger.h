#ifndef _LOGGER_H
#define _LOGGER_H
#include <string>

#define LOG_INFO(logger_ptr, msg)                        \
{                                                        \
  if ((logger_ptr) != nullptr)                           \
    (logger_ptr)->log_info((msg), __FILE__, __LINE__);   \
}

#define LOG_WARNING(logger_ptr, msg)                       \
{                                                          \
  if ((logger_ptr) != nullptr)                             \
    (logger_ptr)->log_warning((msg), __FILE__, __LINE__);  \
}

#define LOG_CRITICAL(logger_ptr, msg)                       \
{                                                        \
  if ((logger_ptr) != nullptr)                              \
    (logger_ptr)->log_critical((msg), __FILE__, __LINE__);  \
}


class AbstractLogger
{
public:
  virtual ~AbstractLogger() = default;

  virtual void log_info(std::string msg, std::string filename, uint64_t line) = 0;

  virtual void log_warning(std::string  msg, std::string filename, uint64_t line) = 0;

  virtual void log_critical(std::string msg, std::string filename, uint64_t line) = 0;
};

#endif // _LOGGER_H
