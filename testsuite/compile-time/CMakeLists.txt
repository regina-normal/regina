# compile-time

# Files to compile
SET ( FILES
  binom
  boolset
  disctype
  facenumbering
  intutils
  link
  perm
  snappea
  triangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} compile-time/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

