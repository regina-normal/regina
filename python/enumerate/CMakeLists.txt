# enumerate

# Files to compile
SET ( FILES
  doubledescription.cpp
  hilbertcd.cpp
  hilbertdual.cpp
  hilbertprimal.cpp
  maxadmissible.cpp
  ordering.cpp
  pyenumerate.cpp
  treeconstraint.cpp
  treelp.cpp
  treetraversal.cpp
  typetrie.cpp
  validityconstraints.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
