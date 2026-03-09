# Files to compile
SET ( FILES
  aggregator.cpp
  base64.cpp
  bitmask.cpp
  boolset.cpp
  stringutils.cpp
  tightencoding.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

