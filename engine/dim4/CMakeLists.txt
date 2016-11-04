
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
