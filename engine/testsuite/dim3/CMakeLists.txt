# Files to compile
SET ( FILES
  decomposition.cpp
  facetpairing3.cpp
  homologicaldata.cpp
  isomorphism3.cpp
  links3.cpp
  triangulation3.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

