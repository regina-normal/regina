
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testall"
  @ONLY
)

ADD_TEST(python-test ./testall)
