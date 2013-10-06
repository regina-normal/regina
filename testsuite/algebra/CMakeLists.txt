# algebra

# Files to compile
SET ( FILES
  nbinarya5
  nbinarys4
  nmvpolynomialring
  nsvpolynomialring
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

