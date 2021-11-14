# enumerate

# Files to compile
SET ( FILES
  pyenumerate.cpp
  treelp.cpp
  typetrie.cpp
  validityconstraints.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
