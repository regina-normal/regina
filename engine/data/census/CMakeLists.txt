
SET(CENSUSDATADIR "${PKGDATADIR}/data/census")

INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})
ADD_EXECUTABLE(mkcensusdb mkcensusdb.cpp)
TARGET_LINK_LIBRARIES(mkcensusdb ${ZLIB_LIBRARIES} ${KVStore_LIBRARIES})
IF (REGINA_KVSTORE_LMDB)
  ADD_EXECUTABLE(optimise-lmdb optimise-lmdb.cpp)
  ADD_EXECUTABLE(compact-lmdb compact-lmdb.cpp)
  TARGET_LINK_LIBRARIES(optimise-lmdb ${KVStore_LIBRARIES})
  TARGET_LINK_LIBRARIES(compact-lmdb ${KVStore_LIBRARIES})
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
  SET(db_1 "${db}.pre1")
  SET(db_2 "${db}.pre2")
  IF (REGINA_KVSTORE_LMDB)
    # For LMDB, we create each database in three stages:
    # 1) copy the records from the .zsig file into a stage 1 LMDB database,
    #    which has the side-effect of sorting the records;
    # 2) re-add these records in sorted order into a stage 2 LMDB database,
    #    which can result in significantly smaller file sizes;
    # 3) ask LMDB to clone this into a final LMDB database using the
    #    MDB_CP_COMPRESS flag, which solves the problem under Windows where
    #    databases would always occupy their entire maximum map size.
    ADD_CUSTOM_COMMAND(OUTPUT "${db_1}" VERBATIM DEPENDS mkcensusdb ${siglist}
      COMMAND mkcensusdb
        "${PROJECT_SOURCE_DIR}/engine/data/census/${siglist}"
        "${PROJECT_BINARY_DIR}/engine/data/census/${db_1}"
    )
    ADD_CUSTOM_COMMAND(OUTPUT "${db_2}" VERBATIM DEPENDS optimise-lmdb ${db_1}
      COMMAND optimise-lmdb
        "${PROJECT_BINARY_DIR}/engine/data/census/${db_1}"
        "${PROJECT_BINARY_DIR}/engine/data/census/${db_2}"
    )
    ADD_CUSTOM_COMMAND(OUTPUT "${db}" VERBATIM DEPENDS compact-lmdb ${db_2}
      COMMAND compact-lmdb
        "${PROJECT_BINARY_DIR}/engine/data/census/${db_2}"
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

