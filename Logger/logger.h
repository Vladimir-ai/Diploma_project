#ifndef LOGGER_H
#define LOGGER_H

class abstract_logger
{
public:
  virtual ~abstract_logger() = default;

  virtual void log_info(char *msg) = 0;
  virtual void log_warning(char *msg) = 0;
  virtual void log_critical(char *msg) = 0;
};

#endif // LOGGER_H
