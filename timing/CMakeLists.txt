
include_directories(${ENGINE_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/engine)

set(TIMING_APPS
  loadlarge
  tree
  )

foreach(APP ${TIMING_APPS})
  string(REGEX REPLACE _ - SOURCE_FILE ${APP})
  add_executable(${APP} ${SOURCE_FILE})
  target_link_libraries(${APP} ${ENGINE_LIBRARY})
endforeach(APP)

