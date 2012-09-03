# circlepack

# Files to compile
SET ( FILES circlePack flower packing colour )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} circlepack/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES colour.h facetData.h flower.h sphereSplit.h draw.h extra.h packing.h vertexAssign.h circlePack.h DESTINATION ${INCLUDEDIR}/circlepack COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
