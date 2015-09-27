  * connect to [Microsoft SQL Server](http://en.wikipedia.org/wiki/Microsoft_SQL_Server) ([Linux 32-bit](http://code.google.com/p/brigantine/wiki/connect_to_ms_sql_linux32) / [Linux 64-bit](http://code.google.com/p/brigantine/wiki/connect_to_ms_sql_linux64) / [Windows](http://code.google.com/p/brigantine/wiki/connect_to_ms_sql_windows))
  * copy layer
  * paste layer -> SQL
  * change text
```
-- CREATE TABLE "ne_110m_admin_0_countries" ("ID" BIGINT, "NAME" NVARCHAR(250), "Geometry" GEOMETRY, PRIMARY KEY ("ID"))
-- CREATE SPATIAL INDEX "ne_110m_admin_0_countries_idx_1" ON "ne_110m_admin_0_countries" ("Geometry") USING GEOMETRY_GRID WITH (BOUNDING_BOX = (-180, -55.6118, 180, 83.6451))
```
> to
```
-- CREATE TABLE "ne_110m_admin_0_countries" ("ID" BIGINT, "NAME" NVARCHAR(250), "Geometry" GEOGRAPHY, PRIMARY KEY ("ID"))
-- CREATE SPATIAL INDEX "ne_110m_admin_0_countries_idx_1" ON "ne_110m_admin_0_countries" ("Geometry") USING GEOGRAPHY_GRID
```
  * uncomment and execute step by step
  * refresh
  * paste rows (try "make counter-clockwise for exterior rings, and clockwise for interior rings" option)