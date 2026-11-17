
CONFIGURE_FILE(
  testone.in
  "${PROJECT_BINARY_DIR}/python/testsuite/testone"
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
CONFIGURE_FILE(
  runbasic.in
  "${PROJECT_BINARY_DIR}/python/testsuite/runbasic"
  @ONLY
)

# A very basic interpreter (and related code) to use for testing:
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS} ${ENGINE_INCLUDES} "${PROJECT_BINARY_DIR}/engine")
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

# A test executable that include _all_ available docstring files, in order to
# ensure that the same docstring constants are not redefined differently.
add_executable(docstrings_all docstrings_all.cpp)
target_link_libraries(docstrings_all ${ENGINE_LIBRARY} ${Python_LIBRARIES})

# Set up the blacklist of tests that should not be used in the PythonBasic
# suite (they will still be used in the main Python suite):
SET(basic_blacklist
  # Cases that expect the python/testsuite directory as an argument:
  file i18n memory1 orb pdf readsiglist repr snappea
  # The following cases implicitly import the threading module, which leads to a
  # harmless but noisy ignored exception upon shutdown under python 3.12 and
  # above.  See: https://github.com/pybind/pybind11/issues/2197
  docstrings)

# Python 3.10.[0-2] cannot process several of our test scripts due to a
# breakage in the line-by-line compilation (this was fixed with the
# introduction of the constant PyCF_ALLOW_INCOMPLETE_INPUT in Python 3.10.3).
# Note: these *are* still tested with testall (which runs within a normal python
# interpreter); just not testbasic (which embeds python in a C++ program).
IF (${Python_VERSION} VERSION_GREATER_EQUAL 3.10.0 AND
    ${Python_VERSION} VERSION_LESS 3.10.3)
  set(basic_blacklist ${basic_blacklist}
    embeddings faces flype groups hypersurfaces iterators
    presentations refs skeleton standardsubcomplex)
ENDIF ()

file(GLOB UTILS_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/*.test")
foreach(TEST_FILE ${UTILS_TESTS})
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

  IF (WIN32)
    # We do not need to add the engine DLL location to the path, since
    # regina-python (used in testone) and testbasic (directly) do this for us.
    ADD_TEST("Python.${TEST_NAME}" bash testone "${TEST_NAME}")
    IF (NOT TEST_NAME IN_LIST basic_blacklist)
      ADD_TEST("PythonBasic.${TEST_NAME}" bash testbasic "${TEST_NAME}")
    ENDIF (NOT TEST_NAME IN_LIST basic_blacklist)
  ELSE (WIN32)
    ADD_TEST("Python.${TEST_NAME}" ./testone "${TEST_NAME}")
    IF (NOT TEST_NAME IN_LIST basic_blacklist)
      ADD_TEST("PythonBasic.${TEST_NAME}" ./testbasic "${TEST_NAME}")
    ENDIF (NOT TEST_NAME IN_LIST basic_blacklist)
  ENDIF (WIN32)
endforeach(TEST_FILE)

IF (WIN32)
  # As above, we do not need to add the engine DLL location to the path.
  ADD_TEST(PythonExec.Sub bash testsub)
  ADD_TEST(PythonExec.Callback bash testcallback)
ELSE (WIN32)
  ADD_TEST(PythonExec.Sub ./testsub)
  ADD_TEST(PythonExec.Callback ./testcallback)
ENDIF (WIN32)
