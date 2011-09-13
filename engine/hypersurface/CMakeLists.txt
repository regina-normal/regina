# hypersurface

# Files to compile
SET ( FILES hsflavours links nhsstandard nnormalhypersurface nnormalhypersurfacelist nxmlhypersurfacereader triangulate )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} hypersurface/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES hsflavourregistry.h nhsmirrored.h nhsstandard.h nnormalhypersurface.h nnormalhypersurfacelist.h nxmlhypersurfacereader.h DESTINATION ${INCLUDEDIR}/hypersurface)
