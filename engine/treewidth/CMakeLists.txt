# Files to compile
SET ( FILES
  treedecomposition
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} treewidth/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    ntreedecomposition.h
    ntreedecomposition-impl.h
    treedecomposition.h
    treedecomposition-impl.h
    DESTINATION ${INCLUDEDIR}/treewidth COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
