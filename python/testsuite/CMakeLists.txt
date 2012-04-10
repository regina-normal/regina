
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testall"
  @ONLY
)

IF (WIN32)
  ADD_TEST(python-test bash testall)
ELSE (WIN32)
  ADD_TEST(python-test ./testall)
ENDIF (WIN32)
