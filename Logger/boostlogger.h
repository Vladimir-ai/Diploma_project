#ifndef BOOSTLOGGER_H
#define BOOSTLOGGER_H

#include "logger.h"



class BoostLogger : public abstract_logger
{
public:
  BoostLogger(std::string path);

  void log_info(std::string msg, std::string filename, uint64_t line);
  void log_warning(std::string msg, std::string filename, uint64_t line);
  void log_critical(std::string msg, std::string filename, uint64_t line);
};

#endif // BOOSTLOGGER_H
