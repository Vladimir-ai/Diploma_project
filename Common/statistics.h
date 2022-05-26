#ifndef STATISTICS_H
#define STATISTICS_H

#include <memory>
#include <vector>
#include <string>
#include <any>
#include <map>
#include <mutex>
#include <variant>

namespace ModulePathProcessor { class PathProcessor; }
using namespace std;

enum stat_type
{
  FEATURE_DETECTOR = 0,
  FEATURE_TRACKER = 1,
  POSE_ESTIMATOR = 2,
  STAT_TYPE_SIZE = 3
};

typedef double VariableType;
typedef VariableType (*AverageFunc)(const vector<VariableType> &vec);


struct StatisticsItem
{
  vector<VariableType> values;
  AverageFunc compute_average_impl = nullptr;
  bool compute_average = false;
};


class Statistics
{
friend class PathDrawerQWidget;
friend class ModulePathProcessor::PathProcessor;

public:
  Statistics();

  void add_statistics(stat_type type, string name, VariableType value, bool compute_average = false, AverageFunc computation = nullptr);

  void remove_last_statistics(stat_type type, string name);

  /* Will remove all data if name var is empty */
  void clear_statistics(stat_type type = STAT_TYPE_SIZE, string name = "");

  map<string, StatisticsItem> get_statistic_by_type(stat_type type = STAT_TYPE_SIZE);
  StatisticsItem get_statistic_by_type_and_name(stat_type type = STAT_TYPE_SIZE, string name = "");

private:
  mutex m_mutex;

  map<string, StatisticsItem> m_stat[STAT_TYPE_SIZE];
  StatisticsItem m_general_stat; // Need it to compute time and etc if needed
};


class IStatisticsImpl
{
  protected:
    Statistics *m_stat;

  public:
    IStatisticsImpl(Statistics *statistics = nullptr)
    {
      m_stat = statistics;
    }

    Statistics *get_statistics(void);
    void set_statistics(Statistics *statistics);
};

#endif // STATISTICS_H
