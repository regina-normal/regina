# hypersurface

# Files to compile
SET ( FILES
  enumerate.cpp
  links.cpp
  hsvectorstandard.cpp
  normalhypersurface.cpp
  normalhypersurfaces.cpp
  xmlhypersurfacereader.cpp
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
    hscoordregistry.h
    hscoordregistry-impl.h
    hsvectorstandard.h
    hypercoords.h
    hyperflags.h
    normalhypersurface.h
    normalhypersurfaces.h
    xmlhypersurfacereader.h
    DESTINATION "${INCLUDEDIR}/hypersurface" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
