# dim3

# Files to compile
#
SET ( FILES
  connectedsumdecomp.cpp
  eltmoves.cpp
  homologicaldata.cpp
  links.cpp
  triangulation3.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

