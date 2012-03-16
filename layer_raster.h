// Andrew Naplavkov

#ifndef LAYER_RASTER_H
#define LAYER_RASTER_H

#include "layer.h"

class layer_raster : public layer {
  brig::database::raster_definition m_raster;

  size_t get_level(const frame& fr) const;
  std::string get_raster_column(size_t level) const;

public:
  layer_raster(connection_link dbc, const brig::database::raster_definition& raster) : layer(dbc), m_raster(raster)  {}
  virtual QString get_string();
  virtual QString get_icon()  { return ":/palette.png"; }

  virtual size_t get_levels()  { return m_raster.levels.size(); }
  virtual brig::database::identifier get_geometry_column(size_t level)  { return m_raster.levels[level].geometry_layer; }
  virtual brig::database::table_definition get_table_definition(size_t level);

  virtual size_t limit()  { return 70; }
  virtual std::shared_ptr<brig::database::rowset> attributes(const frame& fr);
  virtual std::shared_ptr<brig::database::rowset> drawing(const frame& fr, bool limited);
  virtual void draw(const std::vector<brig::database::variant>& row, const frame& fr, QPainter& painter);
}; // layer_raster

#endif // LAYER_RASTER_H
