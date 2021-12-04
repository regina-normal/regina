# surfaces

# Files to compile
SET ( FILES
  boundaries.cpp
  components.cpp
  crushandcut.cpp
  csv.cpp
  disc.cpp
  disctype.cpp
  enumerator.cpp
  links.cpp
  matching.cpp
  normalsurface.cpp
  normalsurfaces.cpp
  surfacefilter.cpp
  orientable.cpp
  quadtostd.cpp
  stdtoquad.cpp
  transform.cpp
  triangles.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} surfaces/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    disc.h
    disctype.h
    normalcoords.h
    normalflags.h
    normalsurface.h
    normalsurfaces.h
    prism.h
    surfacefilter.h
    surfacefiltertype.h
    DESTINATION "${INCLUDEDIR}/surfaces" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
