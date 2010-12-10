# Engine

set ( SOURCES engine shareableobject )
add_subdirectory("algebra")
add_subdirectory("angle")
add_subdirectory("census")
add_subdirectory("data")
add_subdirectory("enumerate")
add_subdirectory("file")
add_subdirectory("foreign")
add_subdirectory("manifold")
add_subdirectory("maths")
add_subdirectory("packet")
add_subdirectory("progress")
add_subdirectory("snappea")
add_subdirectory("split")
add_subdirectory("subcomplex")
add_subdirectory("surfaces")
add_subdirectory("triangulation")
add_subdirectory("utilities")

add_definitions(-DREGINA_DATADIR=\"${PKGDATADIR}\")
add_definitions(-DREGINA_PYLIBDIR=\"${PKGLIBDIR}/python\")

include_directories(${CMAKE_SOURCE_DIR}/engine)
include_directories(${CMAKE_BINARY_DIR}/engine)
add_library("regina-engine" SHARED ${SOURCES} )
add_library("regina-engine_static" STATIC ${SOURCES} )
target_link_libraries(regina-engine xml2 gmp m z pthread)

set( HEADERS ${HEADERS} docs.h engine.h shareableobject.h )
set( MANS ${MANS} regina-engine-config.1 )
set( EXTRA_DIST README.txt checkdocs regina-engine-config.in ${MANS} )


install(FILES ${HEADERS} DESTINATION include)
install(TARGETS regina-engine LIBRARY DESTINATION lib)

