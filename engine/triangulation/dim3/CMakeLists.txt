# Files to compile
SET ( FILES
  angle.cpp
  complement.cpp
  crushtri.cpp
  decompose.cpp
  homology.cpp
  homologicaldata.cpp
  hydrate.cpp
  insertlayered.cpp
  knot.cpp
  reorder.cpp
  retriangulate.cpp
  simplify.cpp
  simplifyglobal.cpp
  skeleton.cpp
  snappea.cpp
  subdivide.cpp
  surfaces.cpp
  triangle3.cpp
  triangulation3.cpp
  turaevviro.cpp
  vertex3.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    component3.h
    homologicaldata.h
    tetrahedron3.h
    triangle3.h
    triangulation3.h
    vertex3.h
    DESTINATION "${INCLUDEDIR}/triangulation/dim3" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
