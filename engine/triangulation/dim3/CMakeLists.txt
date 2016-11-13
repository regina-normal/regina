# Files to compile
SET ( FILES
  angle
  crushtri
  decompose
  homology
  hydrate
  insertlayered
  reorder
  retriangulate
  simplify
  simplifyglobal
  skeleton
  snappea
  subdivide
  surfaces
  triangle3
  triangulation3
  turaevviro
  vertex3
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    boundarycomponent3.h
    component3.h
    tetrahedron3.h
    triangle3.h
    triangulation3.h
    vertex3.h
    DESTINATION ${INCLUDEDIR}/triangulation/dim3 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
