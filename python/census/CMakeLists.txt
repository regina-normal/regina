# census

# Files to compile
SET ( FILES
  census.cpp
  gluingperms2.cpp
  gluingperms3.cpp
  gluingperms4.cpp
  gluingpermsearcher2.cpp
  gluingpermsearcher3.cpp
  gluingpermsearcher4.cpp
  purgeflags.cpp
  pycensus.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

SET ( FILES
  gluingperms5.cpp
  gluingperms6.cpp
  gluingperms7.cpp
  gluingperms8.cpp
  gluingperms9.cpp
  gluingperms10.cpp
  gluingperms11.cpp
  gluingperms12.cpp
  gluingperms13.cpp
  gluingperms14.cpp
  gluingperms15.cpp
  )

IF(NOT LOWDIMONLY)
  # Prepend folder name
  FOREACH ( SOURCE_FILE ${FILES} )
    SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
  ENDFOREACH(SOURCE_FILE)
ENDIF(NOT LOWDIMONLY)

SET(SOURCES ${SOURCES} PARENT_SCOPE)
