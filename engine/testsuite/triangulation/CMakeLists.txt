# Files to compile
SET ( FILES
  exhaustive-tri.cpp
  facenumbering.cpp
  hidim.cpp
  )

ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim3")
ADD_SUBDIRECTORY("dim4")

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
