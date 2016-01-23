SET ( FILES
  boundarycomponentchooser
  cuspchooser
  dim4boundarycomponentchooser
  edgeintchooser
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} choosers/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
