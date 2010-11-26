# utilities

# Files to compile
set ( FILES base64 hashutils i18nutils nbooleans nthread osutils stringutils xmlutils zstream)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( UTILITIES_SOURCES ${SOURCES} PARENT_SCOPE)
