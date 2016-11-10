# Files to compile
SET ( FILES
  simplify
  skeleton
  triangulation2
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/dim2/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    boundarycomponent2.h
    component2.h
    triangle2.h
    triangulation2.h
    DESTINATION ${INCLUDEDIR}/triangulation/dim2 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
