# Engine

SET ( SOURCES globalarray modulemain )
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("srcutils")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/engine ${CMAKE_BINARY_DIR}/engine ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
PYTHON_ADD_MODULE("regina" ${SOURCES} )

# python modules on linux should not have the "lib" prefix 
# (windows doesn't have a prefix anyway)
SET_TARGET_PROPERTIES(regina PROPERTIES PREFIX "")

# python modules on windows (not cygwin though) need a .pyd suffix
IF(WIN32 AND NOT CYGWIN)
  SET_TARGET_PROPERTIES(regina PROPERTIES SUFFIX ".pyd")
ENDIF(WIN32 AND NOT CYGWIN)
TARGET_LINK_LIBRARIES(regina ${ENGINE_LIBRARY} ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})

EXTRA_DIST(README.txt TODO.txt regina-python.in)

# regina-python script
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/python/regina-python.in"
  "${PROJECT_BINARY_DIR}/python/regina-python"
  @ONLY
)

# Copy certain directories into the build tree for in-tree running 
# of regina-python
SET(DIRECTORIES scripts icons)
FOREACH(D ${DIRECTORIES})
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/${D}
    ${CMAKE_BINARY_DIR}/${D}
    )
ENDFOREACH(D)


INSTALL(TARGETS regina LIBRARY DESTINATION lib/regina/python)
INSTALL(FILES regina-python DESTINATION bin)
