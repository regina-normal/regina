
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
ADD_LINK_OPTIONS(${Python_LINK_OPTIONS})
add_executable(basic
  basic.cpp
  ../gui/pythoninterpreter.cpp
  ../gui/pythonoutputstream.cpp)
target_link_libraries(basic ${ENGINE_LIBRARY} ${Python_LIBRARIES})

# We always run the main python test suite (testall).
#
# We only run the embedded interpreter tests (testbasic) if we are
# building the GUI, since this is designed to simulate problems that
# could appear in the GUI's own embedded interpreter.
#
IF (WIN32)
  # We do not need to add the engine DLL location to the path, since
  # regina-python (used in testall) and testbasic (directly) do this for us.
  ADD_TEST(python-test bash testall)
  if (QT_FOUND)
    ADD_TEST(python-basic bash testbasic)
  endif (QT_FOUND)
ELSE (WIN32)
  ADD_TEST(python-test ./testall)
  if (QT_FOUND)
    ADD_TEST(python-basic ./testbasic)
  endif (QT_FOUND)
ENDIF (WIN32)
