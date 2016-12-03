# algebra

# Files to compile
SET ( FILES
  abeliangroup
  grouppresentation
  homgrouppresentation
  markedabeliangroup
  pyalgebra
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

