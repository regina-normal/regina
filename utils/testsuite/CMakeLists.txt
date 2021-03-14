
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/utils/testsuite/testall"
  @ONLY
)

IF (WIN32)
  ADD_TEST(utils-test bash testall)
ELSE (WIN32)
  ADD_TEST(utils-test ./testall)
ENDIF (WIN32)
