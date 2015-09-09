# triangulation 

# Files to compile
SET ( FILES
  angle
  cover
  crushtri
  decompose
  homology
  homotopy
  hydrate
  insertlayered
  isomorphic
  nboundarycomponent
  nedge
  nexampletriangulation
  nfacepair
  nfacepairing
  nhomologicaldata
  ntriangle
  ntriangulation
  nvertex
  nxmltrireader
  reorder
  simplify
  simplifyglobal
  skeleton
  subdivide
  surfaces
  turaevviro
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dimtraits.h
    nboundarycomponent.h
    ncomponent.h
    nedge.h
    nexampletriangulation.h
    nface.h
    nfacepair.h
    nfacepairing.h
    nfacetspec.h
    ngenericisomorphism.h
    ngenericisomorphism.tcc
    nhomologicaldata.h
    nisomorphism.h
    npermit.h
    ntetface.h
    ntetrahedron.h
    ntriangle.h
    ntriangulation.h
    nvertex.h
    nxmltrireader.h
    DESTINATION ${INCLUDEDIR}/triangulation COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
