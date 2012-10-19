// Andrew Naplavkov

#include <iterator>
#include "connection.h"
#include "layer.h"
#include "progress.h"
#include "task_drop.h"

task_drop::task_drop(layer_link lr, bool sql)
  : m_lr(lr), m_sql(sql)
{
}

void task_drop::run(progress* prg)
{
  using namespace std;
  vector<string> sql;
  m_lr->unreg(sql);
  auto dbc(m_lr->get_connection());
  for (size_t level(0), levels(m_lr->get_levels()); level < levels; ++level)
    dbc->drop(m_lr->get_table_definition(level), sql);

  if (m_sql)
    emit signal_commands(dbc, sql);
  else
  {
    auto cmd(dbc->get_command());
    for (auto s(begin(sql)); s != end(sql); ++s)
    {
      cmd->exec(*s);
      if (!prg->step()) return;
    }
    emit signal_refresh(dbc);
  }
}
