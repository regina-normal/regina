# Files to compile
SET ( FILES
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/detail/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    algebra-impl.h
    boundarycomponent.h
    canonical-impl.h
    component.h
    example.h
    face.h
    facenumbering.h
    facetpairing.h
    facetpairing-impl.h
    isosig-impl.h
    retriangulate.h
    simplex.h
    strings.h
    triangulation.h
    DESTINATION "${PKGINCLUDEDIR}/triangulation/detail" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
