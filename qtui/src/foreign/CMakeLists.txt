SET ( FILES
  attachmenthandler.cpp
  csvsurfacehandler.cpp
  dehydrationhandler.cpp
  exportdialog.cpp
  importdialog.cpp
  orbhandler.cpp
  pythonhandler.cpp
  recogniserhandler.cpp
  reginahandler.cpp
  snappeahandler.cpp
  sourcehandler.cpp
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} foreign/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
