SET ( FILES
  commandedit.cpp
  pythonconsole.cpp
  ../../../python/gui/pythoninterpreter.cpp
  ../../../python/gui/pythonoutputstream.cpp
  )
FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
ENDFOREACH(FILE)

SET( SOURCES ${SOURCES} PARENT_SCOPE)
