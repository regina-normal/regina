# enumerate

# Files to compile
SET ( FILES nenumconstraint ndoubledescription ndoubledescriptor nhilbertcd nhilbertdual nhilbertprimal nmaxadmissible )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY("normaliz")

SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES nenumconstraint.h ndoubledescription.h ndoubledescription.tcc ndoubledescriptor.h ndoubledescriptor.tcc nhilbertcd.h nhilbertcd.tcc nhilbertdual.h nhilbertdual.tcc nhilbertprimal.h nhilbertprimal.tcc nmaxadmissible.h nmaxadmissible.tcc ordering.h DESTINATION ${INCLUDEDIR}/enumerate COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
