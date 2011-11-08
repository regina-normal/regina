# enumerate

# Files to compile
SET ( FILES nenumconstraint ndoubledescription ndoubledescriptor nhilbertdual )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES nenumconstraint.h ndoubledescription.h ndoubledescription.tcc ndoubledescriptor.h ndoubledescriptor.tcc nhilbertdual.h nhilbertdual.tcc ordering.h DESTINATION ${INCLUDEDIR}/enumerate)
