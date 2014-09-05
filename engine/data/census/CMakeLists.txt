
SET(CENSUSDATADIR ${PKGDATADIR}/data/census)

INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})
ADD_EXECUTABLE(mkcensusdb mkcensusdb.cpp)
TARGET_LINK_LIBRARIES(mkcensusdb ${Boost_IOSTREAMS_LIBRARY} ${TOKYOCABINET_LIBRARIES})

SET(censusdata_DATA
  closed-hyp-census-full.${DB_EXT}
  closed-nor-census-11.${DB_EXT}
  closed-or-census-11.${DB_EXT}
  cusped-hyp-nor-census-9.${DB_EXT}
  cusped-hyp-or-census-9.${DB_EXT}
  hyp-knot-link-census.${DB_EXT}
  )

FOREACH(db ${censusdata_DATA})
  STRING(REGEX REPLACE [.]${DB_EXT}$ .zsig siglist ${db})
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

