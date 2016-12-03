# surfaces

# Files to compile
SET ( FILES
  disc
  disctype
  normalcoords
  normalflags
  normalsurface
  normalsurfaces
  prism
  surfacefilter
  surfacefiltertype
  pysurfaces
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} surfaces/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
