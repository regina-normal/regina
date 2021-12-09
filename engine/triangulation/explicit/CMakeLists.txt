# Files to compile
SET ( FILES
  boundary2.cpp
  boundary3.cpp
  boundary4.cpp
  boundary5.cpp
  boundary6.cpp
  boundary7.cpp
  boundary8.cpp
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

