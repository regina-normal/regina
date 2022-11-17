
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testall"
  @ONLY
)
CONFIGURE_FILE(
  testbasic.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testbasic"
  @ONLY
)

# A very basic interpreter to use for testing:
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS} ${ENGINE_INCLUDES} "${PROJECT_BINARY_DIR}/engine")
ADD_COMPILE_DEFINITIONS(REGINA_PYTHON_USE_PYTHONPATH=1)
ADD_LINK_OPTIONS(${Python_LINK_OPTIONS})
add_executable(basic
  basic.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic ${ENGINE_LIBRARY} ${Python_LIBRARIES})

IF (WIN32)
  ADD_TEST(python-test bash testall)
  ADD_TEST(python-basic bash testbasic)
  # We do not need to add the engine DLL location to the path, since
  # regina-python (used in testall) and testbasic (directly) do this for us.
ELSE (WIN32)
  ADD_TEST(python-test ./testall)
  ADD_TEST(python-basic ./testbasic)
ENDIF (WIN32)
