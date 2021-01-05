INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS} ${ENGINE_INCLUDES} ${PROJECT_BINARY_DIR}/engine)

SET(LINK_LIBS ${ENGINE_LIBRARY} Qt5::Widgets Qt5::Svg)

ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(choosers)
ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packets)
ADD_SUBDIRECTORY(syntax)

IF(BUILD_PYTHON_BINDINGS)
  ADD_SUBDIRECTORY(python)
  SET(LINK_LIBS ${LINK_LIBS} ${Python_LIBRARIES})
  ADD_LINK_OPTIONS(${Python_LINK_OPTIONS})
  INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS})
ENDIF(BUILD_PYTHON_BINDINGS)

IF(LIBGVC_FOUND)
  IF(${GRAPHVIZ_TYPE} STREQUAL Framework)
    IF(GRAPHVIZ_FRAMEWORK MATCHES "xcframework/macos-")
      # An xcframework.
      GET_FILENAME_COMPONENT(GRAPHVIZ_FRAMEWORK_DIR "${GRAPHVIZ_FRAMEWORK}" DIRECTORY)
      INCLUDE_DIRECTORIES("${GRAPHVIZ_FRAMEWORK_DIR}/Headers")
      SET(LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES} ${GRAPHVIZ_FRAMEWORK})
    ELSE(GRAPHVIZ_FRAMEWORK MATCHES "xcframework/macos-")
      # An ordinary framework.
      INCLUDE_DIRECTORIES("${GRAPHVIZ_FRAMEWORK}/Headers")
      SET(LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES} ${GRAPHVIZ_FRAMEWORK})
    ENDIF(GRAPHVIZ_FRAMEWORK MATCHES "xcframework/macos-")
  ELSEIF(${GRAPHVIZ_TYPE} STREQUAL XCFramework)
    INCLUDE_DIRECTORIES("${GRAPHVIZ_PATH}/Headers")
    SET(LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES} ${GRAPHVIZ_PATH}/libgraphviz.a)
  ELSE (${GRAPHVIZ_TYPE} STREQUAL Framework)
    INCLUDE_DIRECTORIES("${LIBGVC_INCLUDEDIR}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBGVC_CFLAGS_OTHER}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LIBGVC_LDFLAGS_OTHER}")

    # Alas, the pkg-config files shipped with Graphviz under windows are wrong.
    # They give the wrong library directory, and even worse they give the
    # wrong library *names* (graph.dll instead of cgraph.dll).
    # For now, we hard-code fixes for these errors in the WIN32 code below.
    IF(WIN32)
      # Note that, under windows, Graphviz ships its plugins in the same
      # directory as its core library DLLs.
      SET(LINK_LIBS ${LINK_LIBS} gvc cgraph cdt)
      LINK_DIRECTORIES("${LIBGVC_LIBDIR}/release/dll")
    ELSE(WIN32)
      # Assume that pkg-config is telling us the truth.
      # Note, however, that the graphviz plugins typically live in a
      # subdirectory graphviz/ beneath the main library path.
      SET(LINK_LIBS ${LINK_LIBS} ${LIBGVC_LIBRARIES})
      LINK_DIRECTORIES("${LIBGVC_LIBDIR}" "${LIBGVC_LIBDIR}/graphviz")
    ENDIF(WIN32)
    SET(LINK_LIBS ${LINK_LIBS} gvplugin_core gvplugin_dot_layout gvplugin_neato_layout)
  ENDIF (${GRAPHVIZ_TYPE} STREQUAL Framework)
ENDIF(LIBGVC_FOUND)

SET(SOURCES ${SOURCES}
  bigwidget.cpp
  clickablelabel.cpp
  codecchooser.cpp
  columnlayout.cpp
  examplesaction.cpp
  iconcache.cpp
  introdialog.cpp
  main.cpp
  reginamain.cpp
  reginapref.cpp
  reginamanager.cpp
  actionspart
  exports
  imports
  messagelayer
  newpacketdialog
  newpackets
  packetchooser
  packetcreator
  packeteditiface
  packetmanager
  packettabui
  packettreeview
  packetui
  packetwindow
  patiencedialog
  progressdialogs
  treenav
  coordinatechooser
  coordinates
  pythonmanager
  recentfilesaction
  reginaabout
  reginaprefset
  reginasupport
  sharedtempfile
  shortrunner
  )

if (APPLE)
  set(REGINA_APP_NAME Regina)

  # Add application and mimetype icons to the app bundle.
  set(REGINA_MACOS_ICONS
    ${CMAKE_CURRENT_SOURCE_DIR}/macos-icons/Assets.car
    ${CMAKE_CURRENT_SOURCE_DIR}/macos-icons/AppIcon.icns
    ${CMAKE_CURRENT_SOURCE_DIR}/macos-icons/x-regina.icns)
  set_source_files_properties(${REGINA_MACOS_ICONS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set(SOURCES ${SOURCES} ${REGINA_MACOS_ICONS})
else (APPLE)
  set(REGINA_APP_NAME regina-gui)
endif (APPLE)

if (WIN32)
  # Add an icon to the executable.
  set(SOURCES ${SOURCES} regina.rc)
endif (WIN32)

if (${REGINA_INSTALL_TYPE} STREQUAL XDG AND NOT APPLE)
  # Install XDG application icons.
  set (XDGSIZES 16 22 24 32 48 64 128 256)
  foreach (IMG_SIZE ${XDGSIZES})
    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-icons/regina-${IMG_SIZE}.png DESTINATION ${ICONDIR}/hicolor/${IMG_SIZE}x${IMG_SIZE}/apps RENAME regina.png)
    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-icons/x-regina-${IMG_SIZE}.png DESTINATION ${ICONDIR}/hicolor/${IMG_SIZE}x${IMG_SIZE}/mimetypes RENAME application-x-regina.png)
  endforeach (IMG_SIZE ${XDGSIZES})
endif (${REGINA_INSTALL_TYPE} STREQUAL XDG AND NOT APPLE)

set(REGINA_BUNDLE_DEST bin)

ADD_EXECUTABLE(${REGINA_APP_NAME} MACOSX_BUNDLE WIN32 ${SOURCES})

TARGET_LINK_LIBRARIES(${REGINA_APP_NAME} ${LINK_LIBS})

if (APPLE)
  set_target_properties(${REGINA_APP_NAME}
    PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

  # Prepare an Info.plist:
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PACKAGE_VERSION}")

  # This information will be inserted into the following template (which
  # contains file associations for Regina data files, amongst other things):
  set_target_properties(${REGINA_APP_NAME}
    PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/regina.plist )
elseif (LIBGVC_FOUND AND ${GRAPHVIZ_TYPE} STREQUAL Library AND NOT WIN32)
  # Even if this is a no-rpath install into standard system directories,
  # we still need to be able to link with the graphviz plugins.
  # These plugins typically live within a graphviz-specific directory,
  # and so an rpath for these is still required.
  set_target_properties(${REGINA_APP_NAME}
    PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

INSTALL(TARGETS ${REGINA_APP_NAME} DESTINATION ${APPDIR} COMPONENT Runtime)

# TODO: Use COMPONENT Runtime/Development throughout all install targets.

if (${REGINA_INSTALL_TYPE} STREQUAL XDG AND NOT APPLE)
  # Install desktop files and mimetype handling.
  install(FILES regina.desktop DESTINATION ${XDG_APPS_INSTALL_DIR}
    COMPONENT Runtime)
  install(FILES regina.xml DESTINATION ${XDG_MIME_INSTALL_DIR}
    COMPONENT Runtime)
  update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})
endif (${REGINA_INSTALL_TYPE} STREQUAL XDG AND NOT APPLE)

if (WIN32)
  # Install Windows icons for mimetype handling.
  # It seems that, to keep WiX happy, x-regina.ico needs to be in $BINDIR
  # to avoid an ICE69 error (mismatched components for the ProgId entry).
  # I'd love to hear any advice to the contrary.
  install(FILES x-regina.ico DESTINATION ${BINDIR} COMPONENT Runtime)
endif (WIN32)
