// Andrew Naplavkov

#ifndef SQL_THREAD_H
#define SQL_THREAD_H

#include <memory>
#include <QMutex>
#include <QString>
#include <QThread>
#include <QWaitCondition>
#include <queue>
#include <vector>
#include "sql_model.h"
#include "task.h"

class sql_thread : public QThread {
  Q_OBJECT

  std::shared_ptr<sql_model> m_mdl;
  QMutex m_mutex;
  QWaitCondition m_condition;
  std::queue<std::shared_ptr<task>> m_queue;
  bool m_abort, m_cancel;

protected:
  virtual void run();

public slots:
  void push(std::shared_ptr<task> tsk);

signals:
  void signal_start();
  void signal_process(const QString& msg);
  void signal_idle();

public:
  explicit sql_thread(std::shared_ptr<sql_model> mdl);
  virtual ~sql_thread();
  void cancel();
}; // sql_thread

#endif // SQL_THREAD_H