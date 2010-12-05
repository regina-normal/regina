
include_directories(${ENGINE_INCLUDES})

set(UTILS censuscat dehydrate edgelinks equiv identify manifolds normal normal_compare pluggedibundles reduce trisort volume)
foreach(UTIL ${UTILS})
  string(REGEX REPLACE "/_/-/" ${UTIL} SOURCE_FILE)
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY})
endforeach(UTIL)

add_subdirectory(mpi)

set(EXTRA README.txt)
foreach(E ${EXTRA}
  set(EXTRA_D ${CMAKE_CURRENT_SOURCE_DIR}/${E})
endforeach(E)

set(EXTRA_DIST ${EXTRA_DIST} ${EXTRA_D} PARENT_SCOPE)
