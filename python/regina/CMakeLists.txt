# We build the module in this subdirectory so that it can appear as an
# importable python module even when the user runs directly out of the
# source tree.

FILE(COPY __init__.py plainCompleter.py sageSetup.py DESTINATION .)

# Rewrite the source list to be relative to this directory.
FOREACH (SOURCE_FILE ${SOURCES})
  SET (RELSOURCES ${RELSOURCES} ../${SOURCE_FILE})
ENDFOREACH (SOURCE_FILE)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/engine" "${CMAKE_BINARY_DIR}/engine" ${Python_INCLUDE_DIRS})
ADD_LIBRARY("engine" MODULE ${RELSOURCES} )

# Python modules on linux should not have the "lib" prefix
# (windows doesn't have a prefix anyway).
SET_TARGET_PROPERTIES(engine PROPERTIES PREFIX "")

# Enforce a non-standard suffix if we have one.
IF (REGINA_PYTHON_EXTENSION_NONSTANDARD)
  SET_TARGET_PROPERTIES(engine PROPERTIES SUFFIX ".${REGINA_PYTHON_EXTENSION}")
ENDIF (REGINA_PYTHON_EXTENSION_NONSTANDARD)

IF (APPLE)
  # Avoid linking with ${Python_LIBRARIES} so we can load the module in a
  # different python build if we so desire.
  # On macOS this requires the "-undefined dynamic_lookup" linker flag.
  SET_TARGET_PROPERTIES(engine PROPERTIES LINK_FLAGS
    "-undefined dynamic_lookup")
  TARGET_LINK_LIBRARIES(engine ${ENGINE_LIBRARY})
ELSEIF (WIN32)
  # On Windows, just link everything in; Windows is hard enough to
  # support as it is. :/
  TARGET_LINK_LIBRARIES(engine ${ENGINE_LIBRARY} ${Python_LIBRARIES})
ELSE ()
  # On Linux and relations, again do not link with ${Python_LIBRARIES}.
  # This should not require any special linker flags.
  TARGET_LINK_LIBRARIES(engine ${ENGINE_LIBRARY})
ENDIF ()

IF (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # Install the module in python's standard site-packages location.
  # However, we do it directly ourselves (not via distutils), since we
  # want the files to show up in cmake's install_manifest.txt.
  SET(MODULEDIR $ENV{DESTDIR}${Python_SITELIB}/regina)
ELSE (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # Install the module directly ourselves.
  SET(MODULEDIR "${PYLIBDIR}/regina")
ENDIF (${REGINA_INSTALL_TYPE} STREQUAL XDG)

INSTALL(FILES __init__.py plainCompleter.py sageSetup.py DESTINATION "${MODULEDIR}" COMPONENT Runtime)
INSTALL(TARGETS engine LIBRARY DESTINATION "${MODULEDIR}" COMPONENT Runtime)
