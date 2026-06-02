# foreign

# Files to compile
SET ( FILES
  dehydration.cpp
  orb.cpp
  pyforeign.cpp
  siglist.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} foreign/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
