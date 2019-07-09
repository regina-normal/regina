SET ( FILES
  commandedit
  pythonconsole
  pythoninterpreter
  ../../../python/gui/embedding
  )
FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
ENDFOREACH(FILE)

SET( SOURCES ${SOURCES} PARENT_SCOPE)
