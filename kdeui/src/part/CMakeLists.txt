# Engine

SET ( SOURCES actionspart exports flatbutton imports newpacketdialog newpackets packetchooser packetmanager packettabui packettreeview packetui packetwindow patiencedialog progressdialogs reginapart treenav)

ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packettypes)

FOREACH( SOURCE ${SOURCES} )
  SET( PART_SOURCES ${PART_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE} )
ENDFOREACH(SOURCE)
SET( PART_SOURCES ${PART_SOURCES} PARENT_SCOPE )

SET(MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/packettreeview.h ${CMAKE_CURRENT_SOURCE_DIR}/flatbutton.h ${CMAKE_CURRENT_SOURCE_DIR}/newpacketdialog.h ${CMAKE_CURRENT_SOURCE_DIR}/packetchooser.h ${CMAKE_CURRENT_SOURCE_DIR}/packettabui.h ${CMAKE_CURRENT_SOURCE_DIR}/packetui.h ${CMAKE_CURRENT_SOURCE_DIR}/packetwindow.h ${CMAKE_CURRENT_SOURCE_DIR}/patiencedialog.h ${CMAKE_CURRENT_SOURCE_DIR}/progressdialogs.h ${CMAKE_CURRENT_SOURCE_DIR}/reginapart.h)
SET( MOC_HEADERS ${MOC_HEADERS} PARENT_SCOPE )

install(FILES reginapart.rc packetwindow.rc DESTINATION ${DATA_INSTALL_DIR}/regina)
