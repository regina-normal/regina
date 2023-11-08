# link

# Files to compile
SET ( FILES
  examplelink.cpp
  link.cpp
  modellinkgraph.cpp
  pylink.cpp
  spatiallink.cpp
  tangle.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} link/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
