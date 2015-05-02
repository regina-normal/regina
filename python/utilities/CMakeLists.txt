# utilities

# Files to compile
SET ( FILES
  i18nutils
  intutils
  nbooleans
  osutils
  pyutilities
  stringutils
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
