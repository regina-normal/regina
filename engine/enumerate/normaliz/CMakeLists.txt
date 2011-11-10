# enumerate/normaliz

# Files to compile
SET ( FILES libnormaliz )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/normaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# Don't install libnormaliz headers.
