# triangulation 

# Files to compile
SET ( FILES
  example2.cpp
  example3.cpp
  example4.cpp
  facetpairing3.cpp
  facepair.cpp
  homologicaldata.cpp
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
    example.h
    example2.h
    example3.h
    example4.h
    forward.h
    facenumbering.h
    facepair.h
    facetpairing.h
    facetpairing3.h
    facetspec.h
    generic.h
    graph.h
    homologicaldata.h
    isosigencoding.h
    isosigtype.h
    pachner.h
    DESTINATION "${INCLUDEDIR}/triangulation" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
