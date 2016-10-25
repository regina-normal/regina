# triangulation 

# Files to compile
SET ( FILES
  angle
  crushtri
  decompose
  homology
  homotopy
  hydrate
  insertlayered
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
  retriangulate
  simplify
  simplifyglobal
  skeleton
  snappea
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
    dim2.h
    dim3.h
    dim4.h
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
    nxmltrireader.h
    pachner.h
    DESTINATION ${INCLUDEDIR}/triangulation COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
