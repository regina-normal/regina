
SET(CENSUSDATADIR ${PKGDATADIR}/data/census)

SET(censusdata_DATA
  closed-hyp-census-full.tdb
  closed-nor-census-11.tdb
  closed-or-census-11.tdb
  cusped-hyp-nor-census-9.tdb
  cusped-hyp-or-census-9.tdb
  hyp-knot-link-census.tdb
  )
INSTALL(FILES ${censusdata_DATA} DESTINATION ${CENSUSDATADIR} COMPONENT Runtime)

