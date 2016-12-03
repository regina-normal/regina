# dim3

# Files to compile
SET ( FILES
  connectedsumdecomp
  eltmoves
  homologicaldata
  isomorphism
  triangulation3
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

