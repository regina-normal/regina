# triangulation 

# Files to compile
SET ( FILES
  facetpairing3
  nexampletriangulation
  nfacepair
  nhomologicaldata
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(dim2)
ADD_SUBDIRECTORY(dim3)
ADD_SUBDIRECTORY(dim4)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dim2.h
    dim3.h
    dim4.h
    forward.h
    facetpairing.h
    facetpairing3.h
    nboundarycomponent.h
    ncomponent.h
    nedge.h
    nexampletriangulation.h
    nfacepair.h
    nfacepairing.h
    nhomologicaldata.h
    nisomorphism.h
    ntetrahedron.h
    ntriangle.h
    ntriangulation.h
    nvertex.h
    pachner.h
    DESTINATION ${INCLUDEDIR}/triangulation COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
