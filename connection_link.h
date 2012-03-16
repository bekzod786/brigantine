// Andrew Naplavkov

#ifndef CONNECTION_LINK_H
#define CONNECTION_LINK_H

#include <memory>

class connection;

struct connection_link
{
  struct resource {
    connection* dbc;
    resource();
    ~resource();
  }; // resource

  std::shared_ptr<resource> link;

  connection_link();
  bool operator ==(const connection_link& r) const;
  connection* operator ->();
}; // connection_link

#endif // CONNECTION_LINK_H
