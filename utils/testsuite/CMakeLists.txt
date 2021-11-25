
CONFIGURE_FILE(
  testall.in
  "${PROJECT_BINARY_DIR}/utils/testsuite/testall"
  @ONLY
)
CONFIGURE_FILE(
  genout.in
  "${PROJECT_BINARY_DIR}/utils/testsuite/genout"
  @ONLY
)

IF (WIN32)
  ADD_TEST(utils-test bash testall)

  # On Windows we need to add the engine DLL location to the runtime path.
  # The string replacement is to prevent cmake treating $ENV{PATH} as a list.
  string(REPLACE ";" "\\;" ESCAPED_PATH "$ENV{PATH}")
  SET_PROPERTY(TEST utils-test PROPERTY ENVIRONMENT
    "PATH=${ESCAPED_PATH}\;${PROJECT_BINARY_DIR}/engine")
ELSE (WIN32)
  ADD_TEST(utils-test ./testall)
ENDIF (WIN32)
