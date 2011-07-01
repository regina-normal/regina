# Engine

SET ( SOURCES actionspart exports exttabctl flatbutton imports newpacketdialog newpackets packetchooser packetmanager packettabui packettreeview packetui packetwindow patiencedialog progressdialogs reginapart treenav)

ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packettypes)

SET(MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/packettreeview.h ${CMAKE_CURRENT_SOURCE_DIR}/exttabctl.h ${CMAKE_CURRENT_SOURCE_DIR}/flatbutton.h ${CMAKE_CURRENT_SOURCE_DIR}/newpacketdialog.h ${CMAKE_CURRENT_SOURCE_DIR}/packetchooser.h ${CMAKE_CURRENT_SOURCE_DIR}/packettabui.h ${CMAKE_CURRENT_SOURCE_DIR}/packetui.h ${CMAKE_CURRENT_SOURCE_DIR}/packetwindow.h ${CMAKE_CURRENT_SOURCE_DIR}/patiencedialog.h ${CMAKE_CURRENT_SOURCE_DIR}/progressdialogs.h ${CMAKE_CURRENT_SOURCE_DIR}/reginapart.h)

AUTOMOC4_MOC_HEADERS(reginapart ${MOC_HEADERS})
KDE4_ADD_LIBRARY(reginapart SHARED ${COMMON_SOURCES} ${SOURCES})
SET(LINK_LIBS ${LINK_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${ENGINE_LIBRARY} ${KDE4_KTEXTEDITOR_LIBS} ${Boost_PYTHON_LIBRARY})
TARGET_LINK_LIBRARIES(reginapart ${LINK_LIBS})

INSTALL(TARGETS reginapart DESTINATION ${LIB_INSTALL_DIR})
