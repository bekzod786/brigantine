// Andrew Naplavkov

#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include <brig/proj/epsg.hpp>
#include <memory>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QString>
#include <QVariant>
#include <string>
#include <vector>
#include "task.h"
#include "tree_item.h"

class tree_model : public QAbstractItemModel {
  Q_OBJECT

  tree_item m_root;
  size_t m_order;

  void connect_to(connection_link dbc);
  void emit_layers();

private slots:
  void emit_view(const QRectF& rect, const brig::proj::epsg& pj);
  void emit_commands(connection_link dbc, std::vector<std::string> sqls)  { emit signal_commands(dbc, sqls); }
  void refresh(connection_link dbc);

signals:
  void signal_layers(std::vector<layer_link> lrs);
  void signal_view(const QRectF& rect, const brig::proj::epsg& pj);
  void signal_proj(const brig::proj::epsg& pj);
  void signal_commands(connection_link dbc, std::vector<std::string> sqls);
  void signal_task(std::shared_ptr<task> tsk);
  void signal_disconnect(connection_link dbc);

public:
  explicit tree_model(QObject* parent = 0);
  virtual int columnCount(const QModelIndex&) const  { return 1; }
  virtual QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const;
  virtual Qt::ItemFlags flags(const QModelIndex& idx) const;
  virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex& idx) const;
  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
  virtual bool setData(const QModelIndex& idx, const QVariant& value, int role = Qt::EditRole);

  void connect_oci(QString srv, QString usr, QString pwd);
  void connect_odbc(QString dsn);
  void connect_sqlite(QString file, bool init);
  void disconnect(const QModelIndex& idx);
  void refresh(const QModelIndex& idx);
  void use_in_sql(const QModelIndex& idx);
  void paste_layer(layer_link lr_copy, const QModelIndex& idx_paste);
  void zoom_to_fit(const QModelIndex& idx);
  void use_projection(const QModelIndex& idx);
  void paste_rows(layer_link lr_copy, const QModelIndex& idx_paste);
  void drop(const QModelIndex& idx);

  bool is_connection(const QModelIndex& idx) const;
  connection_link get_connection(const QModelIndex& idx) const;
  bool is_layer(const QModelIndex& idx) const;
  layer_link get_layer(const QModelIndex& idx) const;
}; // tree_model

#endif // TREE_MODEL_H
