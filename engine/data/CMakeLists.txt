
SET(INTERNALDATADIR ${PKGDATADIR}/data)

SET(internaldata_DATA README.txt snappea-census-sec5.dat snappea-census-sec6o.dat snappea-census-sec6n.dat snappea-census-sec7o.dat snappea-census-sec7n.dat)

EXTRA_DIST(FILES create-snappea-census-data.py verify-snappea-census-data.py )

INSTALL(FILES ${internaldata_DATA} DESTINATION ${INTERNALDATADIR})

