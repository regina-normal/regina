# file 

# Files to compile
SET ( FILES
  fileformat.cpp
  fileinfo.cpp
  globaldirs.cpp
  pyfile.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} file/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
