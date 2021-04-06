# Files to compile
SET ( FILES
  facehelper.cpp
  facetpairing2.cpp
  facetpairing4.cpp
  facetspec.cpp
  pygeneric.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

SET ( FILES
  boundarycomponent5.cpp
  boundarycomponent6.cpp
  boundarycomponent7.cpp
  boundarycomponent8.cpp
  boundarycomponent9.cpp
  boundarycomponent10.cpp
  boundarycomponent11.cpp
  boundarycomponent12.cpp
  boundarycomponent13.cpp
  boundarycomponent14.cpp
  boundarycomponent15.cpp
  component5.cpp
  component6.cpp
  component7.cpp
  component8.cpp
  component9.cpp
  component10.cpp
  component11.cpp
  component12.cpp
  component13.cpp
  component14.cpp
  component15.cpp
  example5.cpp
  example6.cpp
  example7.cpp
  example8.cpp
  example9.cpp
  example10.cpp
  example11.cpp
  example12.cpp
  example13.cpp
  example14.cpp
  example15.cpp
  face5.cpp
  face6.cpp
  face7.cpp
  face8.cpp
  face9.cpp
  face10.cpp
  face11.cpp
  face12.cpp
  face13.cpp
  face14.cpp
  face15.cpp
  facetpairing5.cpp
  facetpairing6.cpp
  facetpairing7.cpp
  facetpairing8.cpp
  facetpairing9.cpp
  facetpairing10.cpp
  facetpairing11.cpp
  facetpairing12.cpp
  facetpairing13.cpp
  facetpairing14.cpp
  facetpairing15.cpp
  isomorphism5.cpp
  isomorphism6.cpp
  isomorphism7.cpp
  isomorphism8.cpp
  isomorphism9.cpp
  isomorphism10.cpp
  isomorphism11.cpp
  isomorphism12.cpp
  isomorphism13.cpp
  isomorphism14.cpp
  isomorphism15.cpp
  simplex5.cpp
  simplex6.cpp
  simplex7.cpp
  simplex8.cpp
  simplex9.cpp
  simplex10.cpp
  simplex11.cpp
  simplex12.cpp
  simplex13.cpp
  simplex14.cpp
  simplex15.cpp
  triangulation5.cpp
  triangulation6.cpp
  triangulation7.cpp
  triangulation8.cpp
  triangulation9.cpp
  triangulation10.cpp
  triangulation11.cpp
  triangulation12.cpp
  triangulation13.cpp
  triangulation14.cpp
  triangulation15.cpp
  )

IF(NOT LOWDIMONLY)
  # Prepend folder name
  FOREACH ( SOURCE_FILE ${FILES} )
    SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
  ENDFOREACH(SOURCE_FILE)
ENDIF(NOT LOWDIMONLY)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
