SET(SOURCES examplesaction.cpp main.cpp reginamain.cpp reginapref.cpp)
AUTOMOC4_MOC_HEADERS(regina-kde ${MOC_HEADERS} examplesaction.h reginamain.h reginapref.h )
KDE4_ADD_EXECUTABLE(regina-kde ${COMMON_SOURCES} ${SOURCES})
SET(LINK_LIBS ${LINK_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${KDE4_KTEXTEDITOR_LIBS} reginapart)
TARGET_LINK_LIBRARIES(regina-kde ${LINK_LIBS})

INSTALL(TARGETS regina-kde DESTINATION ${BIN_INSTALL_DIR})
