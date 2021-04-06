# link

# Files to compile
#
# Currently we exclude graph.cpp, since we no longer build-depend on boost.
SET ( FILES
  link.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} link/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

