# Files to compile
SET ( FILES
  boundary2.cpp
  boundary3.cpp
  boundary4.cpp
  boundary5.cpp
  boundary6.cpp
  boundary7.cpp
  boundary8.cpp
  facetpairing2.cpp
  facetpairing3.cpp
  facetpairing4.cpp
  facetpairing5.cpp
  facetpairing6.cpp
  facetpairing7.cpp
  facetpairing8.cpp
  linkbuilder.cpp
  triangulation2.cpp
  triangulation3.cpp
  triangulation4.cpp
  triangulation5.cpp
  triangulation6.cpp
  triangulation7.cpp
  triangulation8.cpp
  )
# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/explicit/${SOURCE_FILE})
ENDFOREACH (SOURCE_FILE)

# These files are only included if HIGHDIM is set
SET ( FILES
  boundary9.cpp
  boundary10.cpp
  boundary11.cpp
  boundary12.cpp
  boundary13.cpp
  boundary14.cpp
  boundary15.cpp
  facetpairing9.cpp
  facetpairing10.cpp
  facetpairing11.cpp
  facetpairing12.cpp
  facetpairing13.cpp
  facetpairing14.cpp
  facetpairing15.cpp
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
    SET ( SOURCES ${SOURCES} triangulation/explicit/${SOURCE_FILE})
  ENDFOREACH (SOURCE_FILE)
ENDIF(HIGHDIM)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

