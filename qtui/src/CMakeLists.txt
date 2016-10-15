# Some GUI code will need to know about the installation type.
IF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  ADD_DEFINITIONS(-DREGINA_INSTALL_BUNDLE=1)
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Windows)
  ADD_DEFINITIONS(-DREGINA_INSTALL_WINDOWS=1)
ENDIF ()

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} ${ENGINE_INCLUDES} ${PROJECT_BINARY_DIR}/engine)

SET(LINK_LIBS ${ENGINE_LIBRARY} Qt5::Widgets Qt5::Svg)

ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(choosers)
ADD_SUBDIRECTORY(foreign)
ADD_SUBDIRECTORY(packets)
ADD_SUBDIRECTORY(syntax)

IF(BOOST_PYTHON_FOUND)
  ADD_SUBDIRECTORY(python)
  SET(LINK_LIBS ${LINK_LIBS} ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARY})
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
ENDIF(BOOST_PYTHON_FOUND)

IF(LIBGVC_FOUND)
  IF(${GRAPHVIZ_TYPE} STREQUAL Framework)
    INCLUDE_DIRECTORIES("${GRAPHVIZ_FRAMEWORK}/Headers")
    SET(LINK_LIBS ${LINK_LIBS} ${GRAPHVIZ_FRAMEWORK})
  ELSE (${GRAPHVIZ_TYPE} STREQUAL Framework)
    # Alas, the pkg-config files shipped with Graphviz under windows are wrong.
    # They give the wrong library directory, and even worse they give the
    # wrong library *names* (graph.dll instead of cgraph.dll).
    # For now, we hard-code fixes for these errors in the WIN32 code below.
    INCLUDE_DIRECTORIES("${LIBGVC_INCLUDEDIR}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBGVC_CFLAGS_OTHER}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LIBGVC_LDFLAGS_OTHER}")
    IF(WIN32)
      # We do some hard-coding to fix the pkg-config errors described above.
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
  set(REGINA_ICNS ${CMAKE_CURRENT_SOURCE_DIR}/regina.icns ${CMAKE_CURRENT_SOURCE_DIR}/x-regina.icns)
  set_source_files_properties(${REGINA_ICNS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set(SOURCES ${SOURCES} ${REGINA_ICNS})
else (APPLE)
  set(REGINA_APP_NAME regina-gui)
endif (APPLE)

if (WIN32)
  # Add an icon to the executable.
  set(SOURCES ${SOURCES} regina.rc)
endif (WIN32)

if (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # Install XDG application icons.
  set (XDGSIZES 16 22 24 32 48 64 128 256)
  foreach (IMG_SIZE ${XDGSIZES})
    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-icons/regina-${IMG_SIZE}.png DESTINATION ${ICONDIR}/hicolor/${IMG_SIZE}x${IMG_SIZE}/apps RENAME regina.png)
    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdg-icons/x-regina-${IMG_SIZE}.png DESTINATION ${ICONDIR}/hicolor/${IMG_SIZE}x${IMG_SIZE}/mimetypes RENAME application-x-regina.png)
  endforeach (IMG_SIZE ${XDGSIZES})
endif (${REGINA_INSTALL_TYPE} STREQUAL XDG)

if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  set(REGINA_BUNDLE_DEST .)
else (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  set(REGINA_BUNDLE_DEST bin)
endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

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

if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  # Extra steps for building self-contained MacOSX bundles.

  IF (BOOST_PYTHON_FOUND)
    set (ALL_PLUGINS "${BUNDLEDIR_ESCAPED}/Contents/MacOS/python/regina/engine.so")
  ELSE (BOOST_PYTHON_FOUND)
    set (ALL_PLUGINS)
  ENDIF (BOOST_PYTHON_FOUND)

  foreach (module Core Gui Widgets Svg)
    foreach (plugin ${Qt5${module}_PLUGINS})
      get_target_property(plugin_location ${plugin} LOCATION_Release)
      get_filename_component(plugin_filename "${plugin_location}" NAME)

      # Work out the correct plugins subdirectory.
      get_filename_component(plugin_path "${plugin_location}" PATH)
      get_filename_component(plugin_dirname "${plugin_path}" NAME)
      INSTALL (FILES ${plugin_location}
        DESTINATION ${PLUGINSDIR}/${plugin_dirname}
        COMPONENT Runtime)

      LIST (APPEND ALL_PLUGINS "${PLUGINSDIR_ESCAPED}/${plugin_dirname}/${plugin_filename}")
    endforeach (plugin)
  endforeach (module)

  # Ensure that the bundle is self-contained for linking purposes.
  INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${BUNDLEDIR_ESCAPED}\" \"${ALL_PLUGINS}\" \"\")
    " COMPONENT Runtime)

  SET(FIX_BUNDLE_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/fix-bundle-paths)
  INSTALL(CODE "
    execute_process(COMMAND
      \"${FIX_BUNDLE_PATHS}\" \"${BUNDLEDIR_ESCAPED}\")
    " COMPONENT Runtime)

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
