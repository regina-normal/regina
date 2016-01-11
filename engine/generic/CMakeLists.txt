# generic 

ADD_SUBDIRECTORY("alias")
ADD_SUBDIRECTORY("detail")

# Files to compile
SET ( FILES
  facenumbering
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    component.h
    dimtraits.h
    face.h
    facetpairing.h
    isomorphism.h
    nfacetspec.h
    simplex.h
    triangulation.h
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
