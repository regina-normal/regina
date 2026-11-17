
CONFIGURE_FILE(
  testone.in
  "${PROJECT_BINARY_DIR}/utils/testsuite/testone"
  @ONLY
)
CONFIGURE_FILE(
  genout.in
  "${PROJECT_BINARY_DIR}/utils/testsuite/genout"
  @ONLY
)

file(GLOB UTILS_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/*.test")
foreach(TEST_FILE ${UTILS_TESTS})
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

  IF (WIN32)
    ADD_TEST("Utils.${TEST_NAME}" bash testone "${TEST_NAME}")

    # On Windows we need to add the engine DLL location to the runtime path.
    # The string replacement is to prevent cmake treating $ENV{PATH} as a list.
    string(REPLACE ";" "\\;" ESCAPED_PATH "$ENV{PATH}")
    SET_PROPERTY(TEST "Utils.${TEST_NAME}" PROPERTY ENVIRONMENT
      "PATH=${ESCAPED_PATH}\;${PROJECT_BINARY_DIR}/engine")
  ELSE (WIN32)
    ADD_TEST("Utils.${TEST_NAME}" ./testone "${TEST_NAME}")
  ENDIF (WIN32)
endforeach(TEST_FILE)
