
include_directories(${ENGINE_INCLUDES})

set(UTILS regconcat regconvert regfiledump regfiletype sigcensus tricensus trisetcmp)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE "/_/-/" ${UTIL} SOURCE_FILE)
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY})
endforeach(UTIL)

add_subdirectory(mpi)
add_subdirectory(local)

set(EXTRA README.txt tricensus-manager)
foreach(E ${EXTRA}
  set(EXTRA_D ${CMAKE_CURRENT_SOURCE_DIR}/${E})
endforeach(E)

set(EXTRA_DIST ${EXTRA_DIST} ${EXTRA_D} PARENT_SCOPE)

install(PROGRAMS tricensus-manager DESTINATION bin)
install(TARGETS regconcat regconvert regfiledump regfiletype sigcensus tricensus trisetcmp DESTINATION bin)
