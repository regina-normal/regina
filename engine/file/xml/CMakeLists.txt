# file/xml

# Files to compile
SET ( FILES
  xmlalgebrareader.cpp
  xmlanglestructreader.cpp
  xmlcallback.cpp
  xmlfilterreader.cpp
  xmlhypersurfacereader.cpp
  xmllinkreader.cpp
  xmllinkwriter.cpp
  xmlpacketreader.cpp
  xmlpacketreaders.cpp
  xmlreader.cpp
  xmlsnappeareader.cpp
  xmlsurfacereader.cpp
  xmltrireader2.cpp
  xmltrireader3.cpp
  xmltrireader4.cpp)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} file/xml/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

SET(SOURCES ${SOURCES} PARENT_SCOPE)

# All file/xml/* headers are private: we do not install them.
