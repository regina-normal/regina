# generic/explicit

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
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/explicit/${SOURCE_FILE})
ENDFOREACH (SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

