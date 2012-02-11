SET(regina-app_SRCS examplesaction.cpp main.cpp reginamain.cpp reginapref.cpp reginamanager.cpp)
SET(regina-app_SRCS ${COMMON_SOURCES} ${PART_SOURCES} ${regina-app_SRCS})

# TODO: Restore
# KDE4_ADD_APP_ICON(regina-app_SRCS "../pics/hi*regina.png")

SET ( MOC_HEADERS ${MOC_HEADERS} examplesaction.h reginamain.h reginapref.h)
QT4_WRAP_CPP(regina-app_HEADERS_MOC ${MOC_HEADERS})

if (APPLE)
  set(REGINA_APP_NAME Regina)

  # Add an icon to the app bundle.
  set(REGINA_ICNS "${PROJECT_SOURCE_DIR}/icons/regina.icns")
  set(MACOSX_BUNDLE_ICON_FILE regina.icns)
  set_source_files_properties("${REGINA_ICNS}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set(regina-app_SRCS ${regina-app_SRCS} "${REGINA_ICNS}")
else (APPLE)
  set(REGINA_APP_NAME regina-gui)
endif (APPLE)

if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  set(REGINA_BUNDLE_DEST .)
else (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  set(REGINA_BUNDLE_DEST bin)
endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

ADD_EXECUTABLE(${REGINA_APP_NAME} MACOSX_BUNDLE ${regina-app_SRCS} ${regina-app_HEADERS_MOC})

SET(LINK_LIBS ${LINK_LIBS} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(${REGINA_APP_NAME} ${LINK_LIBS})

INSTALL(TARGETS ${REGINA_APP_NAME}
  BUNDLE DESTINATION ${REGINA_BUNDLE_DEST} COMPONENT Runtime
  RUNTIME DESTINATION bin COMPONENT Runtime)

# TODO: Use COMPONENT Runtime/Development throughout all install targets.

if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  # Extra steps for building self-contained MacOSX bundles.
  set(REGINA_APP_ESCAPED "\${CMAKE_INSTALL_PREFIX}/${REGINA_APP_NAME}.app")

  set(BUNDLE_LIBDIRS ${PROJECT_BINARY_DIR}/engine ${QT_LIBRARY_DIRS})
  INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${REGINA_APP_ESCAPED}\" \"\" \"${BUNDLE_LIBDIRS}\")
    " COMPONENT Runtime)

  # On MacOSX we need a qt.conf file in our bundle; otherwise the system
  # may load both the bundle and system Qt libraries and give unpredictable
  # results.
  # install a qt.conf file# this inserts some cmake code into the install
  # script to write the file
  set(BUNDLE_QT_CONF "${REGINA_APP_ESCAPED}/Contents/Resources/qt.conf")
  INSTALL(CODE "
    file(WRITE \"${BUNDLE_QT_CONF}\" \"\")
    " COMPONENT Runtime)
endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

# TODO: Restore all of these.
# install(FILES regina.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
# install(FILES regina.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
# update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})

