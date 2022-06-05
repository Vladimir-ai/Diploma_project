#include "statistics.h"

Statistics::Statistics()
{
}


void Statistics::add_statistics(stat_type type, string name, VariableType value, bool compute_average, AverageFunc computation)
{
  if (type >= STAT_TYPE_SIZE)
  {
    return;
  }

  m_mutex.lock();

  if (!m_stat[type].count(name))
  {
    m_stat[type][name] = StatisticsItem();
    m_stat[type][name].compute_average_impl = computation;
    m_stat[type][name].compute_average = compute_average;
  }

  m_stat[type][name].values.push_back(value);

  m_mutex.unlock();
}


void Statistics::remove_last_statistics(stat_type type, string name)
{
  if (type >= STAT_TYPE_SIZE)
  {
    return;
  }

  m_mutex.lock();

  if (m_stat[type].count(name))
  {
    m_stat[type][name].values.pop_back();
  }

  m_mutex.unlock();
}


void Statistics::clear_statistics(stat_type type, string name)
{
  m_mutex.lock();

  if (type == STAT_TYPE_SIZE)
  {
    if (!name.length())
    {
      m_general_stat.values.clear();
    }

    for (auto val : m_stat)
    {
      if (name.length())
      {
        val.erase(name);
      }
      else
      {
        val.clear();
      }
    }
  }

  else if (name.length())
  {
    m_stat[type].erase(name);
  }
  else
  {
    m_stat[type].clear();
  }

  m_mutex.unlock();
}


map<string, StatisticsItem> Statistics::get_statistic_by_type(stat_type type)
{
  map<string, StatisticsItem> result;

  m_mutex.lock();
  if (type < STAT_TYPE_SIZE)
  {
    result = m_stat[type];
  }
  m_mutex.unlock();

  return result;
}


StatisticsItem Statistics::get_statistic_by_type_and_name(stat_type type, string name)
{
  StatisticsItem result;

  if (type < STAT_TYPE_SIZE && m_stat[type].count(name))
  {
    result = m_stat[type][name];
  }

  return result;
}


Statistics *IStatisticsImpl::get_statistics()
{
  return m_stat;
}


void IStatisticsImpl::set_statistics(Statistics *statistics)
{
  m_stat = statistics;
}
