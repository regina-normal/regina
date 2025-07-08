
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
CONFIGURE_FILE(
  testsub.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testsub"
  @ONLY
)
CONFIGURE_FILE(
  testcallback.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testcallback"
  @ONLY
)

# A very basic interpreter (and related code) to use for testing:
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS} ${ENGINE_INCLUDES} "${PROJECT_BINARY_DIR}/engine")
ADD_COMPILE_DEFINITIONS(REGINA_PYTHON_USE_PYTHONPATH=1)
ADD_LINK_OPTIONS(${Python_LINK_OPTIONS})
add_executable(basic
  basic.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic ${ENGINE_LIBRARY} ${Python_LIBRARIES})
add_executable(basic_alltypes
  basic_alltypes.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic_alltypes ${ENGINE_LIBRARY} ${Python_LIBRARIES})
add_executable(basic_sub
  basic_sub.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic_sub ${ENGINE_LIBRARY} ${Python_LIBRARIES})
add_executable(basic_callback
  basic_callback.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic_callback ${ENGINE_LIBRARY} ${Python_LIBRARIES})

IF (WIN32)
  ADD_TEST(python-test bash testall)
  ADD_TEST(python-basic bash testbasic)
  ADD_TEST(python-sub bash testsub)
  ADD_TEST(python-callback bash testcallback)
  # We do not need to add the engine DLL location to the path, since
  # regina-python (used in testall) and testbasic (directly) do this for us.
ELSE (WIN32)
  ADD_TEST(python-test ./testall)
  ADD_TEST(python-basic ./testbasic)
  ADD_TEST(python-sub ./testsub)
  ADD_TEST(python-callback ./testcallback)
ENDIF (WIN32)
