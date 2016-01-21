# hypersurface

# Files to compile
SET ( FILES
  enumerate
  links
  nhsstandard
  nnormalhypersurface
  nnormalhypersurfacelist
  nxmlhypersurfacereader
  triangulate
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
    hypercoords.h
    hyperflags.h
    nhsmirrored.h
    nhsstandard.h
    nnormalhypersurface.h
    nnormalhypersurfacelist.h
    nxmlhypersurfacereader.h
    DESTINATION ${INCLUDEDIR}/hypersurface COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
