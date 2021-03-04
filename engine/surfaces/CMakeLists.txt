# surfaces

# Files to compile
SET ( FILES
  boundaries
  components
  crushandcut
  csv
  disc
  disctype
  enumerator
  enumfilter
  links
  normalsurface
  normalsurfaces
  nsvectoranstandard
  nsvectororiented
  nsvectororientedquad
  nsvectorquad
  nsvectorquadclosed
  nsvectorquadoct
  nsvectorquadoctclosed
  nsvectorstandard
  prism
  surfacefilter
  xmlfilterreader
  xmlsurfacereader
  orientable
  quadtostd
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
    disc.h
    disctype.h
    filterregistry.h
    filterregistry-impl.h
    normalcoords.h
    normalflags.h
    normalspec-impl.h
    normalsurface.h
    normalsurfaces.h
    nsvectoranstandard.h
    nsvectormirrored.h
    nsvectororiented.h
    nsvectororientedquad.h
    nsvectorquad.h
    nsvectorquadclosed.h
    nsvectorquadoct.h
    nsvectorquadoctclosed.h
    nsvectorstandard.h
    prism.h
    surfacefilter.h
    surfacefiltertype.h
    xmlfilterreader.h
    xmlsurfacereader.h
    DESTINATION "${INCLUDEDIR}/surfaces" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
