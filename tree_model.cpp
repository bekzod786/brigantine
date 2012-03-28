// Andrew Naplavkov

#include <algorithm>
#include <brig/database/odbc/command_allocator.hpp>
#include <brig/database/oracle/command_allocator.hpp>
#include <brig/database/sqlite/command_allocator.hpp>
#include <exception>
#include <memory>
#include <QFile>
#include <QIcon>
#include <QRegExp>
#include <QString>
#include <QTextStream>
#include <vector>
#include "connection.h"
#include "dialog_create.h"
#include "dialog_drop.h"
#include "dialog_insert.h"
#include "layer.h"
#include "layer_geometry.h"
#include "layer_raster.h"
#include "task_create.h"
#include "task_exec.h"
#include "task_insert.h"
#include "task_mbr.h"
#include "tree_model.h"
#include "utilities.h"

tree_model::tree_model(QObject* parent) : QAbstractItemModel(parent), m_root(0, connection_link()), m_order(0)  {}

QModelIndex tree_model::index(int row, int, const QModelIndex& parent) const
{
  if (parent.isValid() && parent.column() != 0) return QModelIndex();
  const tree_item* parent_itm(parent.isValid()? static_cast<tree_item*>(parent.internalPointer()): &m_root);
  if (row >= (int)parent_itm->m_children.size()) return QModelIndex();
  return createIndex(row, 0, parent_itm->m_children[row].get());
}

QModelIndex tree_model::parent(const QModelIndex& idx) const
{
  if (!idx.isValid()) return QModelIndex();
  const tree_item* itm(static_cast<tree_item*>(idx.internalPointer()));
  const tree_item* parent_itm(itm->m_parent);
  if (!parent_itm) return QModelIndex();
  const int row(parent_itm->position());
  if (row < 0) return QModelIndex();
  return createIndex(row, 0, (void*)parent_itm);
}

int tree_model::rowCount(const QModelIndex& parent) const
{
  const tree_item* parent_itm(parent.isValid()? static_cast<tree_item*>(parent.internalPointer()): &m_root);
  return (int)parent_itm->m_children.size();
}

Qt::ItemFlags tree_model::flags(const QModelIndex& idx) const
{
  Qt::ItemFlags res = (Qt::ItemFlags)0;
  if (idx.isValid())
  {
    res = (Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    if (is_layer(idx)) res |= (Qt::ItemIsTristate|Qt::ItemIsUserCheckable);
  }
  return res;
}

QVariant tree_model::data(const QModelIndex& idx, int role) const
{
  if (idx.isValid())
  {
    tree_item* itm(static_cast<tree_item*>(idx.internalPointer()));
    switch (role)
    {
    case Qt::DecorationRole:
      if (is_connection(idx)) return QIcon(itm->get_connection()->get_icon());
      else if (is_layer(idx)) return QIcon(itm->get_layer()->get_icon());
      break;
    case Qt::DisplayRole: return itm->get_string();
    case Qt::CheckStateRole: if (is_layer(idx)) return itm->get_layer().m_state; break;
    }
  }
  return QVariant();
}

void tree_model::emit_layers()
{
  std::vector<layer_link> lrs;
  for (auto i(std::begin(m_root.m_children)); i != std::end(m_root.m_children); ++i)
  {
    for (auto j(std::begin((*i)->m_children)); j != std::end((*i)->m_children); ++j)
    {
      layer_link lr((*j)->get_layer());
      switch (lr.m_state)
      {
      case Qt::PartiallyChecked:
      case Qt::Checked: lrs.push_back(lr); break;
      case Qt::Unchecked: break;
      }
    }
  }
  emit signal_layers(lrs);
}

bool tree_model::setData(const QModelIndex& idx, const QVariant&, int role)
{
  if (role != Qt::CheckStateRole || !is_layer(idx)) return false;
  tree_item* itm(static_cast<tree_item*>(idx.internalPointer()));
  try  { itm->get_layer()->get_epsg(); }
  catch (const std::exception& e)  { show_message(e.what()); return false; }
  itm->check(++m_order);
  dataChanged(idx, idx);
  emit_layers();
  return true;
}

void tree_model::connect_to(connection_link dbc)
{
  std::unique_ptr<tree_item> dbc_itm(new tree_item(&m_root, dbc));

  auto geometries(dbc->get_geometry_layers());
  for (auto iter(std::begin(geometries)); iter != std::end(geometries); ++iter)
    dbc_itm->m_children.emplace_back(new tree_item(dbc_itm.get(), layer_link(new layer_geometry(dbc, *iter))));

  auto rasters(dbc->get_raster_layers());
  for (auto iter(std::begin(rasters)); iter != std::end(rasters); ++iter)
    dbc_itm->m_children.emplace_back(new tree_item(dbc_itm.get(), layer_link(new layer_raster(dbc, *iter))));

  beginInsertRows(QModelIndex(), int(m_root.m_children.size()), int(m_root.m_children.size()));
  m_root.m_children.push_back(std::move(dbc_itm));
  endInsertRows();
}

void tree_model::connect_oci(QString srv, QString usr, QString pwd)
{
  connect_to(connection_link(new connection
    ( std::make_shared<brig::database::oracle::command_allocator>(srv.toUtf8().constData(), usr.toUtf8().constData(), pwd.toUtf8().constData())
    , srv + ";UID=" + usr + ";"
    )));
}

void tree_model::connect_odbc(QString dsn)
{
  QString str(dsn);
  str.replace(QRegExp("PWD=\\w*;"), "");
  connect_to(connection_link(new connection(std::make_shared<brig::database::odbc::command_allocator>(dsn.toUtf8().constData()), str)));
}

void tree_model::connect_sqlite(QString file, bool init)
{
  class deleter {
    std::shared_ptr<brig::database::command_allocator> m_allocator;
  public:
    explicit deleter(std::shared_ptr<brig::database::command_allocator> allocator) : m_allocator(allocator)  {}
    void operator()(brig::database::command* cmd) const  { m_allocator->deallocate(cmd); }
  }; // deleter

  auto allocator(std::make_shared<brig::database::sqlite::command_allocator>(file.toUtf8().constData()));
  if (init)
  {
    QFile file(":/res/init_spatialite.sql");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream(&file);
    std::unique_ptr<brig::database::command, deleter> cmd(allocator->allocate(), deleter(allocator));
    while (!stream.atEnd())
    {
      const QString sql(stream.readLine());
      cmd->exec(sql.toUtf8().constData());
    }
  }
  connect_to(connection_link(new connection(allocator, file)));
}

bool tree_model::is_connection(const QModelIndex& idx) const
{
  return idx.isValid() && static_cast<tree_item*>(idx.internalPointer())->m_parent == &m_root;
}

connection_link tree_model::get_connection(const QModelIndex& idx) const
{
  return idx.isValid()? static_cast<tree_item*>(idx.internalPointer())->get_connection(): connection_link();
}

bool tree_model::is_layer(const QModelIndex& idx) const
{
  return idx.isValid() && static_cast<tree_item*>(idx.internalPointer())->m_parent != &m_root;
}

layer_link tree_model::get_layer(const QModelIndex& idx) const
{
  return idx.isValid()? static_cast<tree_item*>(idx.internalPointer())->get_layer(): layer_link();
}

void tree_model::disconnect(const QModelIndex& idx)
{
  if (!is_connection(idx)) return;
  tree_item* dbc_itm(static_cast<tree_item*>(idx.internalPointer()));
  bool render(std::find_if
    ( std::begin(dbc_itm->m_children)
    , std::end(dbc_itm->m_children)
    , [&](std::unique_ptr<tree_item>& lr_itm){ return lr_itm->get_layer().m_state != Qt::Unchecked; }
    ) != std::end(dbc_itm->m_children));

  emit signal_disconnect(dbc_itm->get_connection());

  beginRemoveRows(QModelIndex(), idx.row(), idx.row());
  auto iter(std::begin(m_root.m_children));
  std::advance(iter, idx.row());
  m_root.m_children.erase(iter);
  endRemoveRows();

  if (render) emit_layers();
}

void tree_model::refresh(const QModelIndex& idx)
{
  if (!is_connection(idx)) return;
  auto dbc_itm(static_cast<tree_item*>(idx.internalPointer()));
  auto dbc(dbc_itm->get_connection());
  std::vector<std::unique_ptr<tree_item>> children;

  try
  {
    auto geometries(dbc->get_geometry_layers());
    for (auto iter(std::begin(geometries)); iter != std::end(geometries); ++iter)
      children.emplace_back(new tree_item(dbc_itm, layer_link(new layer_geometry(dbc, *iter))));

    auto rasters(dbc->get_raster_layers());
    for (auto iter(std::begin(rasters)); iter != std::end(rasters); ++iter)
      children.emplace_back(new tree_item(dbc_itm, layer_link(new layer_raster(dbc, *iter))));
  }
  catch (const std::exception& e)  { show_message(e.what()); }

  bool render(false);
  for (auto old_iter(std::begin(dbc_itm->m_children)); old_iter != std::end(dbc_itm->m_children); ++old_iter)
  {
    auto old_name((*old_iter)->get_string());
    auto new_iter(std::find_if(std::begin(children), std::end(children), [&](std::unique_ptr<tree_item>& itm){ return old_name  == itm->get_string(); }));
    if (new_iter != std::end(children))
      *new_iter = std::unique_ptr<tree_item>(new tree_item(dbc_itm, (*old_iter)->get_layer()));
    else if ((*old_iter)->get_layer().m_state != Qt::Unchecked)
      render = true;
  }

  if (!dbc_itm->m_children.empty())
  {
    beginRemoveRows(idx, 0, int(dbc_itm->m_children.size() - 1));
    dbc_itm->m_children.clear();
    endRemoveRows();
  }

  if (!children.empty())
  {
    beginInsertRows(idx, 0, int(children.size() - 1));
    std::swap(dbc_itm->m_children, children);
    endInsertRows();
  }

  if (render) emit_layers();
}

void tree_model::zoom_to_fit(const QModelIndex& idx)
{
  if (!is_layer(idx)) return;
  tree_item* lr_itm(static_cast<tree_item*>(idx.internalPointer()));
  layer_link lr(lr_itm->get_layer());
  brig::proj::epsg pj;
  try  { pj = lr->get_epsg(); }
  catch (const std::exception& e)  { show_message(e.what()); return; }
  brig::boost::box box;
  if (lr->get_mbr(box))
    emit signal_view(box_to_rect(box), pj);
  else
  {
    qRegisterMetaType<brig::proj::epsg>("brig::proj::epsg");
    task_mbr* tsk(new task_mbr(lr));
    connect(tsk, SIGNAL(signal_view(QRectF, brig::proj::epsg)), this, SLOT(emit_view(QRectF, brig::proj::epsg)));
    emit signal_task(std::shared_ptr<task>(tsk));
  }
}

void tree_model::emit_view(const QRectF& rect, const brig::proj::epsg& pj)
{
  emit signal_view(rect, pj);
}

void tree_model::use_projection(const QModelIndex& idx)
{
  if (!is_layer(idx)) return;
  tree_item* lr_itm(static_cast<tree_item*>(idx.internalPointer()));
  layer_link lr(lr_itm->get_layer());
  brig::proj::epsg pj;
  try  { pj = lr->get_epsg(); }
  catch (const std::exception& e)  { show_message(e.what()); return; }
  emit signal_proj(pj);
}

void tree_model::use_in_sql(const QModelIndex& idx)
{
  if (!is_connection(idx)) return;
  const tree_item* dbc_itm(static_cast<tree_item*>(idx.internalPointer()));
  emit signal_commands(dbc_itm->get_connection(), std::vector<std::string>());
}

void tree_model::refresh(connection_link dbc)
{
  auto p = std::find_if(std::begin(m_root.m_children), std::end(m_root.m_children)
    , [&](std::unique_ptr<tree_item>& itm){ return itm->get_connection() == dbc; });
  if (p != std::end(m_root.m_children)) refresh(index((*p)->position(), 0));
}

void tree_model::paste_layer(layer_link lr_copy, const QModelIndex& idx_paste)
{
  if (!is_connection(idx_paste)) return;
  const tree_item* dbc_itm(static_cast<tree_item*>(idx_paste.internalPointer()));
  auto dbc(dbc_itm->get_connection());
  dialog_create dlg(lr_copy->get_identifier().name);
  if (dlg.exec() != QDialog::Accepted) return;
  if (dlg.sql())
  {
    qRegisterMetaType<connection_link>("connection_link");
    qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
    task_create* tsk(new task_create(lr_copy, dbc, dlg.tbl(), true));
    connect
      ( tsk, SIGNAL(signal_commands(connection_link, std::vector<std::string>))
      , this, SLOT(emit_commands(connection_link, std::vector<std::string>))
      );
    emit signal_task(std::shared_ptr<task>(tsk));
  }
  else
  {
    qRegisterMetaType<connection_link>("connection_link");
    task_create* tsk(new task_create(lr_copy, dbc, dlg.tbl(), false));
    connect(tsk, SIGNAL(signal_refresh(connection_link)), this, SLOT(refresh(connection_link)));
    emit signal_task(std::shared_ptr<task>(tsk));
  }
}

void tree_model::paste_rows(layer_link lr_copy, const QModelIndex& idx_paste)
{
  if (!is_layer(idx_paste)) return;
  const tree_item* itm_paste(static_cast<const tree_item*>(idx_paste.internalPointer()));
  layer_link lr_paste = itm_paste->get_layer();
  if (lr_copy->get_levels() != lr_paste->get_levels()) return;

  dialog_insert dlg(lr_copy, lr_paste);
  if (dlg.exec() != QDialog::Accepted) return;
  task_insert* tsk(new task_insert(lr_copy, lr_paste, dlg.get_items()));
  emit signal_task(std::shared_ptr<task>(tsk));
}

void tree_model::drop(const QModelIndex& idx)
{
  if (!is_layer(idx)) return;
  tree_item* lr_itm(static_cast<tree_item*>(idx.internalPointer()));
  auto lr(lr_itm->get_layer());
  auto dbc(lr_itm->m_parent->get_connection());

  std::vector<std::string> sql;
  lr->drop_meta(sql);
  for (size_t level(0); level < lr->get_levels(); ++level)
    dbc->drop(lr->get_table_definition(level), sql);

  dialog_drop dlg(lr->get_string());
  if (dlg.exec() != QDialog::Accepted) return;
  else if (dlg.sql()) emit signal_commands(dbc, sql);
  else
  {
    qRegisterMetaType<connection_link>("connection_link");
    task_exec* tsk(new task_exec(dbc, sql));
    connect(tsk, SIGNAL(signal_refresh(connection_link)), this, SLOT(refresh(connection_link)));
    emit signal_task(std::shared_ptr<task>(tsk));
  }
}