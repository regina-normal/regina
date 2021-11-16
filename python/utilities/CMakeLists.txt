# utilities

# Files to compile
SET ( FILES
  base64.cpp
  bitmanip.cpp
  bitmask.cpp
  boolset.cpp
  exception.cpp
  i18nutils.cpp
  intutils.cpp
  osutils.cpp
  pyutilities.cpp
  qitmask.cpp
  randutils.cpp
  stringutils.cpp
  tightencoding.cpp
  trieset.cpp
  xmlutils.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
