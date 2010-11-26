# angle

# Files to compile
set ( FILES nanglestructure nanglestructurelist nxmlanglestructreader )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} angle/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( ANGLE_SOURCES ${SOURCES} PARENT_SCOPE)
