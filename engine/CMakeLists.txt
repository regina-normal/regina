# Engine

# Note: Any flags that need to be picked up by internal compiled engine tools
# (such as mkcensusdb) need to appear *before* the list of subdirectories.

# Pick up generated engine headers (e.g., regina-config.h) as well as
# Regina's standard engine headers.
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/engine)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/engine)

# The tokyocabinet library is only used internally by the engine (i.e., it
# never appears in the engine headers, only the .cpp sources).
# Therefore we include it only here within engine/, not globally.
INCLUDE_DIRECTORIES(${TOKYOCABINET_INCLUDE_DIRS})
LINK_DIRECTORIES(${TOKYOCABINET_LIBRARY_DIRS})

SET ( SOURCES engine )
# Each of these subdirectories modifies the SOURCE variable,
# adding more source files
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("data")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim4")
ADD_SUBDIRECTORY("enumerate")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("generic")
ADD_SUBDIRECTORY("hypersurface")
ADD_SUBDIRECTORY("libnormaliz")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("syntax")
ADD_SUBDIRECTORY("treewidth")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")

# Some C files need to be compiled using the C++ compiler (e.g., snappea,
# where we have added C++ namespaces).
SET_SOURCE_FILES_PROPERTIES( ${SOURCES_C_AS_CPP} PROPERTIES LANGUAGE CXX )

ADD_DEFINITIONS(-DREGINA_DLL_EXPORTS=1)

ADD_LIBRARY("regina-engine" SHARED ${SOURCES} )
# All of the libraries that appear in ENGINE_LINK_LIBRARIES feature in
# the engine headers, which means they may be expose to third-party apps
# that build against libregina-engine.
# Note: PUBLIC and PRIVATE only appeard in CMake 2.8.12.
# For compatibility back to 2.8.7 we use LINK_PUBLIC and LINK_PRIVATE instead.
SET(ENGINE_LINK_LIBRARIES ${LIBXML2_LIBRARIES} ${JANSSON_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(regina-engine
  LINK_PUBLIC ${ENGINE_LINK_LIBRARIES}
  LINK_PRIVATE ${ICONV_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${TOKYOCABINET_LIBRARIES})
SET_TARGET_PROPERTIES(regina-engine PROPERTIES VERSION ${PACKAGE_VERSION})
SET_TARGET_PROPERTIES(regina-engine PROPERTIES SOVERSION ${PACKAGE_VERSION})

SET( HEADERS ${HEADERS} docs.h engine.h output.h regina-core.h )
SET( MANS ${MANS} regina-engine-config.1 )

# Build space-separated lists of flags for regine-engine-config:
GET_DIRECTORY_PROPERTY(ENGINE_INCLUDE_DIRECTORIES DIRECTORY .. INCLUDE_DIRECTORIES)
foreach(arg ${ENGINE_INCLUDE_DIRECTORIES})
  SET(RECONFIG_INCLUDE_FLAGS "${RECONFIG_INCLUDE_FLAGS} -I${arg}")
endforeach(arg ${ENGINE_INCLUDE_DIRECTORIES})
foreach(arg ${ENGINE_LINK_LIBRARIES})
  SET(RECONFIG_LINK_FLAGS "${RECONFIG_LINK_FLAGS} ${arg}")
endforeach(arg ${ENGINE_LINK_LIBRARIES})
CONFIGURE_FILE (
  regina-engine-config.in
  "${PROJECT_BINARY_DIR}/engine/regina-engine-config"
  @ONLY
)

INSTALL(TARGETS regina-engine
  LIBRARY DESTINATION ${LIBDIR}
  RUNTIME DESTINATION ${BINDIR}
  COMPONENT Runtime)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES ${HEADERS} DESTINATION ${INCLUDEDIR} COMPONENT Development)
  INSTALL(FILES ${CMAKE_BINARY_DIR}/engine/regina-config.h DESTINATION ${INCLUDEDIR} COMPONENT Development)
  INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/engine/regina-engine-config DESTINATION bin COMPONENT Development)
  INSTALL(FILES regina-engine-config.1 DESTINATION ${MANDIR}/man1 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})

# Miscellaneous extras:
ADD_SUBDIRECTORY("doxygen")
