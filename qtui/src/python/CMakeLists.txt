SET ( FILES
  commandedit
  pythonconsole
  ../../../python/gui/pythoninterpreter
  ../../../python/gui/pythonoutputstream
  )
FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
ENDFOREACH(FILE)

SET( SOURCES ${SOURCES} PARENT_SCOPE)
