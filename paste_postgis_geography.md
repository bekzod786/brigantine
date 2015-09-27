  * look at [Microsoft SQL Server](http://code.google.com/p/brigantine/wiki/paste_ms_sql_geography)
  * change text
```
-- CREATE TABLE "ne_110m_admin_0_countries" ("ID" BIGINT, "NAME" VARCHAR(250), PRIMARY KEY ("ID"))
-- SELECT AddGeometryColumn('ne_110m_admin_0_countries', 'Geometry', (SELECT SRID FROM PUBLIC.SPATIAL_REF_SYS WHERE AUTH_NAME LIKE 'EPSG' AND AUTH_SRID = 4326 ORDER BY SRID FETCH FIRST 1 ROWS ONLY), 'GEOMETRY', 2)
-- CREATE INDEX "ne_110m_admin_0_countries_idx_1" ON "ne_110m_admin_0_countries" USING GIST("Geometry")
```
> to
```
-- CREATE TABLE "ne_110m_admin_0_countries" ("ID" BIGINT, "NAME" VARCHAR(250), PRIMARY KEY ("ID"), "Geometry" GEOGRAPHY(MULTIPOLYGON, 4326))
-- CREATE INDEX "ne_110m_admin_0_countries_idx_1" ON "ne_110m_admin_0_countries" USING GIST("Geometry")
```