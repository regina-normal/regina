
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testall"
  @ONLY
)

IF (WIN32)
  ADD_TEST(python-test bash testall)
  # We do not need to add the engine DLL location to the path, since
  # regina-python does this for us.
ELSE (WIN32)
  ADD_TEST(python-test ./testall)
ENDIF (WIN32)
