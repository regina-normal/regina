# surfaces

# Files to compile
SET ( FILES
  components
  crushandcut
  csv
  enumerator
  enumfilter
  links
  ndisc
  ndisctype
  nnormalsurface
  nnormalsurfacelist
  nprism
  nsanstandard
  nsoriented
  nsorientedquad
  nsquad
  nsquadoct
  nsstandard
  nsurfacefilter
  nsurfacesubset
  nxmlfilterreader
  nxmlfilterreaders
  nxmlsurfacereader
  orientable
  quadtostd
  sfcombination
  sfproperties
  spheres
  stdtoquad
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} surfaces/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    coordregistry.h
    coordregistry-impl.h
    filterregistry.h
    filterregistry-impl.h
    flavourregistry.h
    ndisc.h
    ndisctype.h
    nnormalsurface.h
    nnormalsurfacelist.h
    normalcoords.h
    normalflags.h
    normalspec-impl.h
    nprism.h
    nsanstandard.h
    nsmirrored.h
    nsoriented.h
    nsorientedquad.h
    nsquad.h
    nsquadoct.h
    nsstandard.h
    nsurfacefilter.h
    nsurfacesubset.h
    nxmlfilterreader.h
    nxmlsurfacereader.h
    sfcombination.h
    sfproperties.h
    surfacefiltertype.h
    DESTINATION ${INCLUDEDIR}/surfaces COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
