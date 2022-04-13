#include "boostlogger.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

BoostLogger::BoostLogger(std::string path)
{
  boost::log::add_file_log(
        boost::log::keywords::file_name = path,
        boost::log::keywords::format = "%TimeStamp%: [%Severity%] %Message%"
  );

  boost::log::add_common_attributes();

//
//  boost::log::formatter formatter =
//      boost::log::expressions::stream
//      << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f/ ")
//      << boost::log::expressions::attr<boost::thread::id>("ThreadID")
//      << boost::log::expressions::smessage;

//  ptr->set_formatter(formatter);
}

void BoostLogger::log_info(std::string msg, std::string filename, uint64_t line)
{
  BOOST_LOG_TRIVIAL(info) << filename << ":" << line << ": " << msg;
}

void BoostLogger::log_warning(std::string msg, std::string filename, uint64_t line)
{
  BOOST_LOG_TRIVIAL(warning) << filename << ":" << line << ": " << msg;
}

void BoostLogger::log_critical(std::string msg, std::string filename, uint64_t line)
{
  BOOST_LOG_TRIVIAL(fatal) << filename << ":" << line << ": " << msg;
}
