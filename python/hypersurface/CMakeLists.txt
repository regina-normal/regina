# hypersurface

# Files to compile
SET ( FILES
  hypercoords
  hyperflags
  normalhypersurface
  normalhypersurfaces
  pyhypersurface
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} hypersurface/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
