# angle

# Files to compile
SET ( FILES
  angleflags.cpp
  anglestructure.cpp
  anglestructures.cpp
  pyangle.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} angle/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
