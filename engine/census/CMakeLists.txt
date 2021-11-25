# census

# Files to compile
SET ( FILES
  census.cpp
  closedprimemin.cpp
  compact.cpp
  euler.cpp
  gluingpermsearcher2.cpp
  gluingpermsearcher3.cpp
  gluingpermsearcher4.cpp
  hyperbolic.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(explicit)

SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    census.h
    census-impl.h
    gluingperms.h
    gluingperms-impl.h
    gluingpermsearcher.h
    gluingpermsearcher2.h
    gluingpermsearcher3.h
    gluingpermsearcher4.h
    DESTINATION "${INCLUDEDIR}/census" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
