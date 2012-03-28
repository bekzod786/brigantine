// Andrew Naplavkov

#ifndef MAP_THREAD_H
#define MAP_THREAD_H

#include <QImage>
#include <QMutex>
#include <QThread>
#include <QTime>
#include <QWaitCondition>
#include <vector>
#include "frame.h"
#include "layer_link.h"

class map_thread : public QThread {
  Q_OBJECT

  QMutex m_mutex;
  QWaitCondition m_condition;
  bool m_abort, m_restart;
  std::vector<layer_link> m_lrs;
  frame m_fr;

  void render_layer(layer_link lr, const frame& fr, QImage& img, QString& msg, size_t& rows, QTime& time);

protected:
  virtual void run();

signals:
  void signal_start();
  void signal_process(const frame& fr, const QImage& image);
  void signal_process(const QString& msg);
  void signal_idle();

public:
  explicit map_thread(QObject* parent = 0);
  virtual ~map_thread();
  void render(std::vector<layer_link> lrs, const frame& fr);
}; // map_thread

#endif // MAP_THREAD_H