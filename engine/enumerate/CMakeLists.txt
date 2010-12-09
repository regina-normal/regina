# enumerate

# Files to compile
set ( FILES nenumconstraint ndoubledescription ndoubledescriptor)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES nenumconstraint.h ndoubledescription.h ndoubledescription.tcc ndoubledescriptor.h ndoubledescriptor.tcc DESTINATION ${INCLUDEDIR}/enumerate}
