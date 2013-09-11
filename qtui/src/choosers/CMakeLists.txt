# Engine

SET ( FILES boundarycomponentchooser edgechooser edgeintchooser facechooser tetrahedronchooser vertexchooser )

SET ( MOC_FILES boundarycomponentchooser.h edgechooser.h edgeintchooser.h facechooser.h tetrahedronchooser.h vertexchooser.h )

FOREACH( MOC ${MOC_FILES} )
  SET ( MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/${MOC} )
ENDFOREACH(MOC)
SET ( MOC_HEADERS ${MOC_HEADERS} PARENT_SCOPE)

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} choosers/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
