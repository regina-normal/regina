# maths/spec

# Files to compile
SET ( FILES
  perm3.cpp
  perm4.cpp
  perm5.cpp
  perm6.cpp
  perm7.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/spec/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    perm2.h
    perm3.h
    perm4.h
    perm5.h
    perm6.h
    perm7.h
    DESTINATION "${INCLUDEDIR}/maths/spec" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
