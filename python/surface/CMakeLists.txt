# surface

# Files to compile
SET ( FILES
  disc.cpp
  disctype.cpp
  normalcoords.cpp
  normalflags.cpp
  normalsurface.cpp
  normalsurfaces.cpp
  prism.cpp
  surfacefilter.cpp
  surfacefiltertype.cpp
  pysurface.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} surface/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
