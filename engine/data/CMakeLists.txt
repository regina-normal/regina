
SET(INTERNALDATADIR ${PKGDATADIR}/internal/data)

SET(internaldata_DATA snappea-census-sec5.dat snappea-census-sec6o.dat snappea-census-sec6n.dat snappea-census-sec7o.dat snappea-census-sec7n.dat)
INSTALL(FILES ${internaldata_DATA} DESTINATION ${INTERNALDATADIR} COMPONENT Runtime)

