SET ( FILES
  abstracthighlighter.cpp
  qtmatcher.cpp
  syntaxhighlighter.cpp
  )
FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)

