# algebra

# Files to compile
SET ( FILES
  grouppresentation
  bilinearform
  binarya5
  binarys4
  cellulardata
  markedabeliangroup
  mvpolynomialring
  svpolynomialring
  fibring
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

