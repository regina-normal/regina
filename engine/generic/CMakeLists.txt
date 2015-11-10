# generic 

# Files to compile
SET ( FILES
  ngenerictriangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    canonical-impl.h
    component.h
    dimtraits.h
    facetpairing.h
    facetpairing-impl.h
    isomorphism.h
    isomorphism-impl.h
    isosig-impl.h
    nfacetspec.h
    ngenerictriangulation.h
    simplex.h
    triangulation.h
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
