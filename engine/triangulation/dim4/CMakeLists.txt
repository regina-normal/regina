# Files to compile
SET ( FILES
  edge4.cpp
  retriangulate.cpp
  simplify.cpp
  simplifyglobal.cpp
  skeleton.cpp
  subdivide.cpp
  triangulation4.cpp
  vertex4.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/dim4/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    component4.h
    edge4.h
    pentachoron4.h
    triangulation4.h
    vertex4.h
    DESTINATION "${INCLUDEDIR}/triangulation/dim4" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
