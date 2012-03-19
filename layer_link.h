// Andrew Naplavkov

#ifndef LAYER_LINK_H
#define LAYER_LINK_H

#include <memory>
#include <Qt>

class layer; // https://svn.boost.org/trac/boost/ticket/6687

struct layer_link {
  struct resource {
    layer* lr;
    resource();
    ~resource();
  }; // resource

  std::shared_ptr<resource> link;
  Qt::CheckState m_state;
  size_t m_order;

  layer_link();
  layer* operator ->() const;
}; // layer_link

#endif // LAYER_LINK_H
