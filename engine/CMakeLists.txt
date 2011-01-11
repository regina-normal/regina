# Engine

SET ( SOURCES engine shareableobject )
# Each of these subdirectories modifies the SOURCE variable,
# adding more source files
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("data")
ADD_SUBDIRECTORY("enumerate")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")

ADD_DEFINITIONS(-DREGINA_DATADIR=\"${PKGDATADIR}\")
ADD_DEFINITIONS(-DREGINA_PYLIBDIR=\"${PKGLIBDIR}/python\")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/engine)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/engine)
ADD_LIBRARY("regina-engine" SHARED ${SOURCES} )
ADD_LIBRARY("regina-engine_static" STATIC ${SOURCES} )
TARGET_LINK_LIBRARIES(regina-engine xml2 gmp m z pthread)
TARGET_LINK_LIBRARIES(regina-engine_static xml2 gmp m z pthread)

SET( HEADERS ${HEADERS} docs.h engine.h shareableobject.h )
SET( MANS ${MANS} regina-engine-config.1 )
SET( EXTRA_DIST README.txt checkdocs regina-engine-config.in ${MANS} )


INSTALL(FILES ${HEADERS} DESTINATION ${INCLUDEDIR})
INSTALL(TARGETS regina-engine LIBRARY DESTINATION lib)

