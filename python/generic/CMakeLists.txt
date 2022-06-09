# Files to compile
SET ( FILES
  cut.cpp
  example5.cpp
  example6.cpp
  example7.cpp
  example8.cpp
  face5.cpp
  face6.cpp
  face7.cpp
  face8.cpp
  facehelper.cpp
  facetpairing2.cpp
  facetpairing4.cpp
  facetpairing5.cpp
  facetpairing6.cpp
  facetpairing7.cpp
  facetpairing8.cpp
  facetspec.cpp
  isomorphism5.cpp
  isomorphism6.cpp
  isomorphism7.cpp
  isomorphism8.cpp
  pygeneric.cpp
  triangulation5.cpp
  triangulation6.cpp
  triangulation7.cpp
  triangulation8.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# These files are only included if HIGHDIM is set
SET ( FILES
  example9.cpp
  example10.cpp
  example11.cpp
  example12.cpp
  example13.cpp
  example14.cpp
  example15.cpp
  face9.cpp
  face10.cpp
  face11.cpp
  face12.cpp
  face13.cpp
  face14.cpp
  face15.cpp
  facetpairing9.cpp
  facetpairing10.cpp
  facetpairing11.cpp
  facetpairing12.cpp
  facetpairing13.cpp
  facetpairing14.cpp
  facetpairing15.cpp
  isomorphism9.cpp
  isomorphism10.cpp
  isomorphism11.cpp
  isomorphism12.cpp
  isomorphism13.cpp
  isomorphism14.cpp
  isomorphism15.cpp
  triangulation9.cpp
  triangulation10.cpp
  triangulation11.cpp
  triangulation12.cpp
  triangulation13.cpp
  triangulation14.cpp
  triangulation15.cpp
  )

IF(HIGHDIM)
  # Prepend folder name
  FOREACH ( SOURCE_FILE ${FILES} )
    SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
  ENDFOREACH(SOURCE_FILE)
ENDIF(HIGHDIM)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
