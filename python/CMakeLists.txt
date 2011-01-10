# Engine

set ( SOURCES globalarray modulemain )
add_subdirectory("algebra")
add_subdirectory("angle")
add_subdirectory("census")
add_subdirectory("file")
add_subdirectory("foreign")
add_subdirectory("manifold")
add_subdirectory("maths")
add_subdirectory("packet")
add_subdirectory("progress")
add_subdirectory("snappea")
add_subdirectory("split")
add_subdirectory("srcutils")
add_subdirectory("subcomplex")
add_subdirectory("surfaces")
add_subdirectory("triangulation")
add_subdirectory("utilities")


include_directories(${CMAKE_SOURCE_DIR}/engine)
include_directories(${CMAKE_BINARY_DIR}/engine)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})
PYTHON_ADD_MODULE("regina" ${SOURCES} )
set_target_properties(regina PROPERTIES PREFIX "")
if(WIN32 AND NOT CYGWIN)
  set_target_properties(regina PROPERTIES PREFIX ".pyd")
endif()
target_link_libraries(regina ${ENGINE_LIBRARY} ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})

extra_dist(README.txt TODO.txt regina-python.in)
configure_file (
  "${PROJECT_SOURCE_DIR}/python/regina-python.in"
  "${PROJECT_BINARY_DIR}/python/regina-python"
  @ONLY
)

# Copy certain directories into the build tree for in-tree running
set(DIRECTORIES scripts icons)
foreach(D ${DIRECTORIES})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/${D}
    ${CMAKE_BINARY_DIR}/${D}
    )
endforeach(D)


install(TARGETS regina LIBRARY DESTINATION lib/regina/python)
install(FILES regina-python DESTINATION bin)
