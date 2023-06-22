# Engine

# Note: Any flags that need to be picked up by internal compiled engine tools
# (such as mkcensusdb) need to appear *before* the list of subdirectories.

# Pick up generated engine headers (e.g., regina-config.h) as well as
# Regina's standard engine headers.
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/engine")
INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}/engine")

# The key-value store is only used internally by the engine (i.e., it
# never appears in the engine headers, only the .cpp sources).
# Therefore we include it only here within engine/, not globally.
INCLUDE_DIRECTORIES(${KVStore_INCLUDE_DIRS})
LINK_DIRECTORIES(${KVStore_LIBRARY_DIRS})

SET ( SOURCES )
# Each of these subdirectories modifies the SOURCE variable,
# adding more source files
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("core")
ADD_SUBDIRECTORY("data")
ADD_SUBDIRECTORY("enumerate")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("hypersurface")
ADD_SUBDIRECTORY("libnormaliz")
ADD_SUBDIRECTORY("link")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surface")
ADD_SUBDIRECTORY("syntax")
ADD_SUBDIRECTORY("treewidth")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")

# Ensure that, when building under Windows, the linker is given the full list
# of symbols to export in the DLL.  This avoids the need to manage attributes
# such as __declspec(dllexport) and __declspec(dllimport) within the C++ code.
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

ADD_LIBRARY("regina-engine" SHARED ${SOURCES} )
# All of the libraries that appear in ENGINE_LINK_LIBRARIES feature in
# the engine headers, which means they may be expose to third-party apps
# that build against libregina-engine.
# Note: PUBLIC and PRIVATE only appeard in CMake 2.8.12.
# For compatibility back to 2.8.7 we use LINK_PUBLIC and LINK_PRIVATE instead.
SET(ENGINE_LINK_LIBRARIES ${LIBXML2_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(regina-engine
  LINK_PUBLIC ${ENGINE_LINK_LIBRARIES}
  LINK_PRIVATE ${STDFS_LIBRARY} "${ICONV_LIBRARY}" ${ZLIB_LIBRARIES} ${KVStore_LIBRARIES})
SET_TARGET_PROPERTIES(regina-engine PROPERTIES VERSION ${PACKAGE_VERSION})
SET_TARGET_PROPERTIES(regina-engine PROPERTIES SOVERSION ${PACKAGE_VERSION})

# Give regina-helper access to the library filename.
SET ( ENGINE_FILENAME
  "${CMAKE_SHARED_LIBRARY_PREFIX}regina-engine${CMAKE_SHARED_LIBRARY_SUFFIX}" )

SET( HEADERS ${HEADERS} docs.h regina-core.h )
SET( MANS ${MANS} regina-engine-config.1 )

# Build space-separated lists of flags for regine-engine-config and
# regina-helper.  Each variable should be a space-separated list of flags,
# suitable for passing on a bash command line in an unquoted context.
GET_DIRECTORY_PROPERTY(ENGINE_INCLUDE_DIRECTORIES DIRECTORY .. INCLUDE_DIRECTORIES)
foreach(arg ${ENGINE_INCLUDE_DIRECTORIES})
  REGINA_ESCAPE_BASH_BARE (arg)
  SET(RECONFIG_INCLUDE_FLAGS "${RECONFIG_INCLUDE_FLAGS} -I${BASH_BARE_arg}")
endforeach(arg ${ENGINE_INCLUDE_DIRECTORIES})
foreach(arg ${ENGINE_LINK_LIBRARIES})
  REGINA_ESCAPE_BASH_BARE (arg)
  SET(RECONFIG_LINK_FLAGS "${RECONFIG_LINK_FLAGS} ${BASH_BARE_arg}")
endforeach(arg ${ENGINE_LINK_LIBRARIES})
REGINA_ESCAPE_BASH (RECONFIG_INCLUDE_FLAGS)
REGINA_ESCAPE_PERL (RECONFIG_INCLUDE_FLAGS)
REGINA_ESCAPE_BASH (RECONFIG_LINK_FLAGS)
REGINA_ESCAPE_PERL (RECONFIG_LINK_FLAGS)
CONFIGURE_FILE (
  regina-engine-config.in
  "${PROJECT_BINARY_DIR}/engine/regina-engine-config"
  @ONLY
)
CONFIGURE_FILE (
  regina-helper.in
  "${PROJECT_BINARY_DIR}/engine/regina-helper"
  @ONLY
)

INSTALL(TARGETS regina-engine
  LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
  COMPONENT Runtime)
INSTALL(PROGRAMS "${PROJECT_BINARY_DIR}/engine/regina-helper" DESTINATION bin)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES ${HEADERS} DESTINATION "${PKGINCLUDEDIR}" COMPONENT Development)
  INSTALL(FILES "${CMAKE_BINARY_DIR}/engine/regina-config.h" DESTINATION "${PKGINCLUDEDIR}" COMPONENT Development)
  INSTALL(PROGRAMS "${PROJECT_BINARY_DIR}/engine/regina-engine-config" DESTINATION bin COMPONENT Development)
  INSTALL(FILES regina-engine-config.1 DESTINATION "${CMAKE_INSTALL_FULL_MANDIR}/man1" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})

# Miscellaneous extras:
ADD_SUBDIRECTORY("doxygen")
ADD_SUBDIRECTORY("testsuite")
