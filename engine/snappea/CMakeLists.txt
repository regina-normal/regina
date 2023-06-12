# snappea 

# Files to compile
SET ( FILES
  fundgroup.cpp
  homology.cpp
  examplesnappea.cpp
  snappeatriangulation.cpp
  uimessages.cpp)


# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(kernel)
ADD_SUBDIRECTORY(snappy)


# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    examplesnappea.h
    snappeatriangulation.h
    DESTINATION "${PKGINCLUDEDIR}/snappea" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})

