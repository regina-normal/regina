
SET(CENSUSDATADIR ${PKGDATADIR}/data/census)

INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})
ADD_EXECUTABLE(mkcensusdb mkcensusdb.cpp ../../utilities/zstream.cpp)
TARGET_LINK_LIBRARIES(mkcensusdb ${ZLIB_LIBRARIES} ${TOKYOCABINET_LIBRARIES})

SET(censusdata_DATA
  closed-hyp-census-full.tdb
  closed-nor-census-11.tdb
  closed-or-census-11.tdb
  cusped-hyp-nor-census-9.tdb
  cusped-hyp-or-census-9.tdb
  hyp-knot-link-census.tdb
  )

FOREACH(db ${censusdata_DATA})
  STRING(REGEX REPLACE [.]tdb$ .dat.gz siglist ${db})
  ADD_CUSTOM_COMMAND(OUTPUT ${db} DEPENDS mkcensusdb ${siglist}
    COMMAND mkcensusdb
      ${PROJECT_SOURCE_DIR}/engine/data/census/${siglist}
      ${PROJECT_BINARY_DIR}/engine/data/census/${db}
  )
  INSTALL(
    FILES ${PROJECT_BINARY_DIR}/engine/data/census/${db}
    DESTINATION ${CENSUSDATADIR}
    COMPONENT Runtime)
ENDFOREACH(db)

ADD_CUSTOM_TARGET(censusdata ALL DEPENDS ${censusdata_DATA})

