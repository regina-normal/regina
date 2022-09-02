
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testall"
  @ONLY
)

# A very basic interpreter to use for testing:
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS} ${ENGINE_INCLUDES} "${PROJECT_BINARY_DIR}/engine")
ADD_LINK_OPTIONS(${Python_LINK_OPTIONS})
add_executable(basic
  basic.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic ${ENGINE_LIBRARY} ${Python_LIBRARIES})

IF (WIN32)
  ADD_TEST(python-test bash testall)
  # We do not need to add the engine DLL location to the path, since
  # regina-python does this for us.
ELSE (WIN32)
  ADD_TEST(python-test ./testall)
ENDIF (WIN32)
