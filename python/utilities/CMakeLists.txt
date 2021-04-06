# utilities

# Files to compile
SET ( FILES
  boolset.cpp
  i18nutils.cpp
  intutils.cpp
  osutils.cpp
  pyutilities.cpp
  randutils.cpp
  stringutils.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
