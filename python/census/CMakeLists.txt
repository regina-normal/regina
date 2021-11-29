# census

# Files to compile
SET ( FILES
  census.cpp
  gluingperms2.cpp
  gluingperms3.cpp
  gluingperms4.cpp
  gluingperms5.cpp
  gluingperms6.cpp
  gluingperms7.cpp
  gluingperms8.cpp
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

# These files are only included if HIGHDIM is set
SET ( FILES
  gluingperms9.cpp
  gluingperms10.cpp
  gluingperms11.cpp
  gluingperms12.cpp
  gluingperms13.cpp
  gluingperms14.cpp
  gluingperms15.cpp
  )

IF(HIGHDIM)
  # Prepend folder name
  FOREACH ( SOURCE_FILE ${FILES} )
    SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
  ENDFOREACH(SOURCE_FILE)
ENDIF(HIGHDIM)

SET(SOURCES ${SOURCES} PARENT_SCOPE)
