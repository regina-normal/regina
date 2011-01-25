# Engine

SET ( SOURCES actionspart exports exttabctl flatbutton gridlistview imports newpacketdialog newpackets packetchooser packetmanager packettabui packettreeview packetui packetwindow patiencedialog progressdialogs reginapart treenav)

ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packettypes)
AUTOMOC4_MOC_HEADERS(reginapart ${MOC_HEADERS})
KDE4_ADD_LIBRARY(reginapart ${COMMON_SOURCES} ${SOURCES})

TARGET_LINK_LIBRARIES(reginapart ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${LIB_ENGINE})

INSTALL(TARGETS reginapart DESTINATION ${LIB_INSTALL_DIR})
