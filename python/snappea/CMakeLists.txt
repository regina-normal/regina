# snappea 

# Files to compile
set ( FILES nsnappeatriangulation pysnappea)


# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} snappea/${SOURCE_FILE})
endforeach(SOURCE_FILE)


# Set the variable in the parent directory
set(SOURCES ${SOURCES} PARENT_SCOPE)

