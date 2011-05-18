
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testall"
  @ONLY
)

#ADD_TEST(
#  NAME python-test
#  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/python/testsuite
#  COMMAND ./testall
#)
