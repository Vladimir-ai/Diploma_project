#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class abstract_logger
{
public:
  virtual ~abstract_logger() = default;

  virtual void log_info(std::string msg) = 0;
  virtual void log_warning(std::string  msg) = 0;
  virtual void log_critical(std::string msg) = 0;
};

#endif // LOGGER_H
