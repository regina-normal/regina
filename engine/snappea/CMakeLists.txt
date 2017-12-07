# snappea 

# Files to compile
SET ( FILES
  fundgroup
  homology
  examplesnappea
  snappeatriangulation
  xmlsnappeareader
  uimessages)


# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(kernel)
ADD_SUBDIRECTORY(snappy)


# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
SET(SOURCES_C_AS_CPP ${SOURCES_C_AS_CPP} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    examplesnappea.h
    nexamplesnappeatriangulation.h
    nsnappeatriangulation.h
    snappeatriangulation.h
    xmlsnappeareader.h
    DESTINATION ${INCLUDEDIR}/snappea COMPONENT Development)
endif (${REGINA_INSTALL_DEV})

