# snappea 

# Files to compile
SET ( FILES nsnappeatriangulation pysnappea)


# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)


# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

