
SET(CENSUSDATADIR "${PKGDATADIR}/data/census")

INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})
ADD_EXECUTABLE(mkcensusdb mkcensusdb.cpp)
TARGET_LINK_LIBRARIES(mkcensusdb ${ZLIB_LIBRARIES} ${KVSTORE_LIBRARIES})
IF (REGINA_KVSTORE_LMDB)
  ADD_EXECUTABLE(optimise-lmdb optimise-lmdb.cpp)
  ADD_EXECUTABLE(compact-lmdb compact-lmdb.cpp)
  TARGET_LINK_LIBRARIES(optimise-lmdb ${KVSTORE_LIBRARIES})
  TARGET_LINK_LIBRARIES(compact-lmdb ${KVSTORE_LIBRARIES})
ENDIF (REGINA_KVSTORE_LMDB)

SET(censusdata_DATA
  closed-hyp-census-full.${DB_EXT}
  closed-nor-census-11.${DB_EXT}
  closed-or-census-11.${DB_EXT}
  cusped-hyp-nor-census-9.${DB_EXT}
  cusped-hyp-or-census-9.${DB_EXT}
  christy-knots-links.${DB_EXT}
  )

FOREACH(db ${censusdata_DATA})
  STRING(REGEX REPLACE [.]${DB_EXT}$ .zsig siglist ${db})
  SET(db_pre "${db}.pre")
  IF (REGINA_KVSTORE_LMDB)
    # For LMDB, we create each database in two stages:
    # 1) copy the records from the .zsig file into a preliminary LMDB database,
    #    which has the side-effect of sorting the records by key;
    # 2) copy these records into a second LMDB database, now processing
    #    them in sorted order by key, which in practice seems to result in a
    #    significantly smaller file size.
    ADD_CUSTOM_COMMAND(OUTPUT "${db_pre}" VERBATIM DEPENDS mkcensusdb ${siglist}
      COMMAND mkcensusdb
        "${PROJECT_SOURCE_DIR}/engine/data/census/${siglist}"
        "${PROJECT_BINARY_DIR}/engine/data/census/${db_pre}"
    )
    ADD_CUSTOM_COMMAND(OUTPUT "${db}" VERBATIM DEPENDS optimise-lmdb ${db_pre}
      COMMAND optimise-lmdb
        "${PROJECT_BINARY_DIR}/engine/data/census/${db_pre}"
        "${PROJECT_BINARY_DIR}/engine/data/census/${db}"
    )
  ELSE (REGINA_KVSTORE_LMDB)
    # Both Tokyo Cabinet and QDBM have optimisation functions in their APIs,
    # which our mkcensusdb tool calls directly.  We can therefore create
    # the final database from the source .zsig file in a single step.
    ADD_CUSTOM_COMMAND(OUTPUT ${db} VERBATIM DEPENDS mkcensusdb ${siglist}
      COMMAND mkcensusdb
        "${PROJECT_SOURCE_DIR}/engine/data/census/${siglist}"
        "${PROJECT_BINARY_DIR}/engine/data/census/${db}"
    )
  ENDIF (REGINA_KVSTORE_LMDB)
  INSTALL(
    FILES "${PROJECT_BINARY_DIR}/engine/data/census/${db}"
    DESTINATION "${CENSUSDATADIR}"
    COMPONENT Runtime)
ENDFOREACH(db)

ADD_CUSTOM_TARGET(censusdata ALL DEPENDS ${censusdata_DATA})

