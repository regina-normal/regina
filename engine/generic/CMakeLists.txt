# generic 

# Files to compile
SET ( FILES
  ngenericisomorphism
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
    dimtraits.h
    isomorphism-impl.h
    isosig-impl.h
    nfacetspec.h
    ngenericisomorphism-impl.h
    ngenericisomorphism.h
    ngenerictriangulation.h
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
