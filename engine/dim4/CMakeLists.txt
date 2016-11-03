# dim4 

# Files to compile
SET ( FILES
  dim4exampletriangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim4/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dim4boundarycomponent.h
    dim4component.h
    dim4edge.h
    dim4exampletriangulation.h
    dim4facetpairing.h
    dim4triangle.h
    dim4isomorphism.h
    dim4pentachoron.h
    dim4tetrahedron.h
    dim4triangulation.h
    dim4vertex.h
    DESTINATION ${INCLUDEDIR}/dim4 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
