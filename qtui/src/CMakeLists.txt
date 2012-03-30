INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Some GUI code will need to know about the installation type.
IF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  ADD_DEFINITIONS(-DREGINA_INSTALL_BUNDLE=1)
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Windows)
  ADD_DEFINITIONS(-DREGINA_INSTALL_WINDOWS=1)
ENDIF ()

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} ${ENGINE_INCLUDES} ${PROJECT_BINARY_DIR}/engine)

SET(LINK_LIBS ${ENGINE_LIBRARY} ${QT_LIBRARIES})

ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packettypes)
IF(BOOST_PYTHON_FOUND)
  ADD_SUBDIRECTORY(python)
  SET(LINK_LIBS ${LINK_LIBS} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
ENDIF(BOOST_PYTHON_FOUND)
IF(SRCHILITE_FOUND)
  ADD_SUBDIRECTORY(srchiliteqt)
  ADD_SUBDIRECTORY(syntax)
  SET(LINK_LIBS ${LINK_LIBS} ${SRCHILITE_LIBRARY})
  INCLUDE_DIRECTORIES(${SRCHILITE_INCLUDE_DIR})
ELSE(SRCHILITE_FOUND)
  ADD_DEFINITIONS(-DNO_SRCHILITE=1)
ENDIF(SRCHILITE_FOUND)

SET(SOURCES ${SOURCES} bigwidget.cpp codecchooser.cpp examplesaction.cpp iconcache.cpp introdialog.cpp main.cpp reginamain.cpp reginapref.cpp reginamanager.cpp actionspart exports imports messagelayer newpacketdialog newpackets packetchooser packetcreator packeteditiface packetmanager packettabui packettreeview packetui packetwindow patiencedialog progressdialogs treenav coordinatechooser coordinates pythonmanager recentfilesaction reginaabout reginaprefset reginasupport sharedtempfile shortrunner)

SET(MOC_HEADERS ${MOC_HEADERS} coordinatechooser.h introdialog.h recentfilesaction.h reginaprefset.h shortrunner.h packettreeview.h newpacketdialog.h packetchooser.h packeteditiface.h packettabui.h packetui.h packetwindow.h patiencedialog.h progressdialogs.h examplesaction.h reginaabout.h reginamain.h reginamanager.h reginapref.h)

QT4_WRAP_CPP(regina-app_HEADERS_MOC ${MOC_HEADERS})

if (APPLE)
  set(REGINA_APP_NAME Regina)

  # Add application and mimetype icons to the app bundle.
  set(REGINA_ICNS ${PROJECT_SOURCE_DIR}/icons/regina.icns ${PROJECT_SOURCE_DIR}/icons/x-regina.icns)
  set_source_files_properties(${REGINA_ICNS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set(SOURCES ${SOURCES} ${REGINA_ICNS})
else (APPLE)
  set(REGINA_APP_NAME regina-gui)
endif (APPLE)

if (WIN32)
  # Add an icon to the executable.
  set(SOURCES ${SOURCES} regina.rc)
endif (WIN32)

if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  set(REGINA_BUNDLE_DEST .)
else (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  set(REGINA_BUNDLE_DEST bin)
endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

ADD_EXECUTABLE(${REGINA_APP_NAME} MACOSX_BUNDLE WIN32 ${SOURCES} ${regina-app_HEADERS_MOC})

TARGET_LINK_LIBRARIES(${REGINA_APP_NAME} ${LINK_LIBS})

if (APPLE)
  # Prepare an Info.plist:
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PACKAGE_VERSION}")

  # This information will be inserted into the following template (which
  # contains file associations for Regina data files, amongst other things):
  set_target_properties(${REGINA_APP_NAME}
    PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/regina.plist )
endif (APPLE)

INSTALL(TARGETS ${REGINA_APP_NAME} DESTINATION ${APPDIR} COMPONENT Runtime)

# TODO: Use COMPONENT Runtime/Development throughout all install targets.

if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  # Extra steps for building self-contained MacOSX bundles.
  set(BUNDLE_LIBDIRS ${QT_LIBRARY_DIRS})
  IF (BOOST_PYTHON_FOUND)
    INSTALL(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${BUNDLEDIR_ESCAPED}\" \"python/regina.so\" \"${BUNDLE_LIBDIRS}\")
      " COMPONENT Runtime)
  ELSE (BOOST_PYTHON_FOUND)
    INSTALL(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${BUNDLEDIR_ESCAPED}\" \"\" \"${BUNDLE_LIBDIRS}\")
      " COMPONENT Runtime)
  ENDIF (BOOST_PYTHON_FOUND)

  # On MacOSX we need a qt.conf file in our bundle; otherwise the system
  # may load both the bundle and system Qt libraries and give unpredictable
  # results.
  set(BUNDLE_QT_CONF "${BUNDLEDIR_ESCAPED}/Contents/Resources/qt.conf")
  INSTALL(CODE "
    file(WRITE \"${BUNDLE_QT_CONF}\" \"\")
    " COMPONENT Runtime)
endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

if (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # Install desktop files and mimetype handling.
  install(FILES regina.desktop DESTINATION ${XDG_APPS_INSTALL_DIR}
    COMPONENT Runtime)
  install(FILES regina.xml DESTINATION ${XDG_MIME_INSTALL_DIR}
    COMPONENT Runtime)
  update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
endif (${REGINA_INSTALL_TYPE} STREQUAL XDG)

if (WIN32)
  # Install Windows icons for mimetype handling.
  # It seems that, to keep WiX happy, x-regina.ico needs to be in $BINDIR
  # to avoid an ICE69 error (mismatched components for the ProgId entry).
  # I'd love to hear any advice to the contrary.
  install(FILES x-regina.ico DESTINATION ${BINDIR} COMPONENT Runtime)
endif (WIN32)

if (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # Install a placeholder for the old regina-kde.
  ADD_EXECUTABLE(regina-kde WIN32 regina-kde)
  TARGET_LINK_LIBRARIES(regina-kde ${QT_LIBRARIES})
  INSTALL(TARGETS regina-kde DESTINATION ${APPDIR} COMPONENT Runtime)
  INSTALL(FILES regina-kde.1 DESTINATION ${MANDIR}/man1 COMPONENT Runtime)
endif (${REGINA_INSTALL_TYPE} STREQUAL XDG)
