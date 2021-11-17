# census

# Files to compile
SET ( FILES
  census.cpp
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
SET(SOURCES ${SOURCES} PARENT_SCOPE)
