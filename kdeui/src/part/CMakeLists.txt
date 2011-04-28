# Engine

SET ( SOURCES actionspart exports exttabctl flatbutton imports newpacketdialog newpackets packetchooser packetmanager packettabui packettreeview packetui packetwindow patiencedialog progressdialogs reginapart treenav)

ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packettypes)

SET(MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/exttabctl.h ${CMAKE_CURRENT_SOURCE_DIR}/flatbutton.h ${CMAKE_CURRENT_SOURCE_DIR}/newpacketdialog.h ${CMAKE_CURRENT_SOURCE_DIR}/packetchooser.h)

AUTOMOC4_MOC_HEADERS(reginapart ${MOC_HEADERS})
KDE4_ADD_LIBRARY(reginapart ${COMMON_SOURCES} ${SOURCES})

TARGET_LINK_LIBRARIES(reginapart ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${LIB_ENGINE})

INSTALL(TARGETS reginapart DESTINATION ${LIB_INSTALL_DIR})
