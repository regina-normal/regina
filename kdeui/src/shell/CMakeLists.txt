SET(regina-kde_SRCS examplesaction.cpp main.cpp reginamain.cpp reginapref.cpp)
SET(regina-kde_SRCS ${COMMON_SOURCES} ${PART_SOURCES} ${regina-kde_SRCS})

KDE4_ADD_APP_ICON(regina-kde_SRCS "../pics/hi*regina.png")

AUTOMOC4_MOC_HEADERS(regina-kde ${MOC_HEADERS} examplesaction.h reginamain.h reginapref.h )
KDE4_ADD_EXECUTABLE(regina-kde ${regina-kde_SRCS})
SET(LINK_LIBS ${LINK_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${KDE4_KTEXTEDITOR_LIBS})
TARGET_LINK_LIBRARIES(regina-kde ${LINK_LIBS})

INSTALL(TARGETS regina-kde DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES regina.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES reginamain.rc tips DESTINATION ${DATA_INSTALL_DIR}/regina)
install(FILES regina.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})

