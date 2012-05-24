# Engine

SET ( FILES csvsurfacehandler dehydrationhandler exportdialog importdialog isosighandler orbhandler pdfhandler pythonhandler recogniserhandler reginahandler snappeahandler sourcehandler )

SET ( MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/exportdialog.h ${CMAKE_CURRENT_SOURCE_DIR}/importdialog.h PARENT_SCOPE)

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} foreign/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
