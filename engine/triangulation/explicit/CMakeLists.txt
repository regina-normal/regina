# Files to compile
SET ( FILES
  boundary2
  boundary3
  boundary4
  boundary5
  boundary6
  boundary7
  boundary8
  boundary9
  boundary10
  boundary11
  boundary12
  boundary13
  boundary14
  boundary15
  triangulation2
  triangulation3
  triangulation4
  triangulation5
  triangulation6
  triangulation7
  triangulation8
  triangulation9
  triangulation10
  triangulation11
  triangulation12
  triangulation13
  triangulation14
  triangulation15
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/explicit/${SOURCE_FILE})
ENDFOREACH (SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

