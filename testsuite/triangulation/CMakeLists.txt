# triangulation

# Files to compile
SET ( FILES
  connectedsumdecomp
  eltmoves
  nhomologicaldata
  nisomorphism
  ntriangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

