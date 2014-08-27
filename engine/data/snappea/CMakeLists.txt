
SET(SNAPPEADATADIR ${PKGDATADIR}/data/snappea)

SET(snappeadata_DATA
  snappea-census-sec5.dat
  snappea-census-sec6o.dat
  snappea-census-sec6n.dat
  snappea-census-sec7o.dat
  snappea-census-sec7n.dat
  )
INSTALL(FILES ${snappeadata_DATA} DESTINATION ${SNAPPEADATADIR} COMPONENT Runtime)

