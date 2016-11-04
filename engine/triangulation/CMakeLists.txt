# triangulation 

# Files to compile
SET ( FILES
  example2
  example3
  example4
  facenumbering
  facetpairing3
  nfacepair
  nhomologicaldata
  xmltri2reader
  xmltri3reader
  xmltri4reader
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(alias)
ADD_SUBDIRECTORY(detail)
ADD_SUBDIRECTORY(dim2)
ADD_SUBDIRECTORY(dim3)
ADD_SUBDIRECTORY(dim4)
ADD_SUBDIRECTORY(explicit)
ADD_SUBDIRECTORY(generic)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dim2.h
    dim3.h
    dim4.h
    example2.h
    example3.h
    example4.h
    forward.h
    facenumbering.h
    facetpairing.h
    facetpairing3.h
    facetspec.h
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
    xmltrireader.h
    xmltri2reader.h
    xmltri3reader.h
    xmltri4reader.h
    DESTINATION ${INCLUDEDIR}/triangulation COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
