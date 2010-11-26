# Engine

set ( ENGINE_SOURCES engine shareableobject )
set ( ALGEBRA_SOURCES )
add_subdirectory("algebra")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${ALGEBRA_SOURCES} )

add_subdirectory("angle")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${ANGLE_SOURCES} )

add_subdirectory("census")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${CENSUS_SOURCES} )

add_subdirectory("data")

add_subdirectory("enumerate")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${ENUMERATE_SOURCES} )

add_subdirectory("file")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${FILE_SOURCES} )

add_subdirectory("foreign")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${FOREIGN_SOURCES} )

add_subdirectory("manifold")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${MANIFOLD_SOURCES} )

add_subdirectory("maths")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${MATHS_SOURCES} )

add_subdirectory("packet")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${PACKET_SOURCES} )

add_subdirectory("progress")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${PROGRESS_SOURCES} )

add_subdirectory("snappea")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${SNAPPEA_SOURCES} )

add_subdirectory("split")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${SPLIT_SOURCES} )

add_subdirectory("subcomplex")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${SUBCOMPLEX_SOURCES} )

add_subdirectory("surfaces")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${SURFACES_SOURCES} )

add_subdirectory("triangulation")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${TRIANGULATION_SOURCES} )

add_subdirectory("utilities")
set ( ENGINE_SOURCES ${ENGINE_SOURCES} ${UTILITIES_SOURCES} )

add_definitions(-DREGINA_DATADIR=\"${PKGDATADIR}\")
add_definitions(-DREGINA_PYLIBDIR=\"${PKGLIBDIR}/python\")

include_directories(${CMAKE_SOURCE_DIR}/engine)
include_directories(${CMAKE_BINARY_DIR}/engine)
message(STATUS "Adding ${ENGINE_SOURCES}")
add_library("regina-engine" SHARED ${ENGINE_SOURCES} )
target_link_libraries(regina-engine m gmp)
