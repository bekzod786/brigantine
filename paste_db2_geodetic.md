  * IBM DB2 Geodetic Data Management Feature has been discontinued. This feature was deprecated in Version 9.7.
  * look at [Microsoft SQL Server](http://code.google.com/p/brigantine/wiki/paste_ms_sql_geography)
  * change text
```
-- CREATE TABLE "ne_110m_admin_0_countries" ("ID" BIGINT NOT NULL, "NAME" VARGRAPHIC(250), "Geometry" DB2GSE.ST_GEOMETRY, PRIMARY KEY ("ID"))
-- BEGIN ATOMIC DECLARE msg_code INTEGER; DECLARE msg_text VARCHAR(1024); call DB2GSE.ST_register_spatial_column(NULL, '"ne_110m_admin_0_countries"', '"Geometry"', (SELECT SRS_NAME FROM DB2GSE.ST_SPATIAL_REFERENCE_SYSTEMS WHERE ORGANIZATION LIKE 'EPSG' AND ORGANIZATION_COORDSYS_ID = 4326 ORDER BY SRS_ID FETCH FIRST 1 ROWS ONLY), msg_code, msg_text); END
-- CREATE INDEX "ne_110m_admin_0_countries_idx_1" ON "ne_110m_admin_0_countries" ("Geometry") EXTEND USING DB2GSE.SPATIAL_INDEX (1, 0, 0)
```
> to
```
-- CREATE TABLE "ne_110m_admin_0_countries" ("ID" BIGINT NOT NULL, "NAME" VARGRAPHIC(250), "Geometry" DB2GSE.ST_MULTIPOLYGON, PRIMARY KEY ("ID"))
-- BEGIN ATOMIC DECLARE msg_code INTEGER; DECLARE msg_text VARCHAR(1024); call DB2GSE.ST_register_spatial_column(NULL, '"ne_110m_admin_0_countries"', '"Geometry"', 'GEODETIC_GCS_WGS_1984', msg_code, msg_text); END
-- CREATE INDEX "ne_110m_admin_0_countries_idx_1" ON "ne_110m_admin_0_countries" ("Geometry") EXTEND USING DB2GSE.SPATIAL_INDEX (-1, -1, 1)
```