# hypersurface

# Files to compile
SET ( FILES
  enumerate.cpp
  links.cpp
  matching.cpp
  normalhypersurface.cpp
  normalhypersurfaces.cpp
  tetrahedra.cpp
  triangulate.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} hypersurface/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    hypercoords.h
    hyperflags.h
    normalhypersurface.h
    normalhypersurfaces.h
    DESTINATION "${PKGINCLUDEDIR}/hypersurface" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
