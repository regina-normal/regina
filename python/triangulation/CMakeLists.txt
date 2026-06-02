
# Files to compile
SET ( FILES
  cut.cpp
  facenumbering.cpp
  facetspec.cpp
  isosig.cpp
  pytriangulation.cpp
  )

ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim3")
ADD_SUBDIRECTORY("dim4")
ADD_SUBDIRECTORY("hidim")

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
