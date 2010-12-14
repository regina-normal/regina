
include_directories(${ENGINE_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/engine)

find_package(POPT REQUIRED)
include_directories(${POPT_INCLUDE_DIR})

set(UTILS regconcat regconvert regfiledump regfiletype sigcensus tricensus trisetcmp)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${POPT_LIBRARY})
endforeach(UTIL)

if(MPI_FOUND)
  add_subdirectory(mpi)
endif(MPI_FOUND)
add_subdirectory(local)

extra_dist(FILES README.txt)

install(PROGRAMS tricensus-manager DESTINATION bin)
install(TARGETS regconcat regconvert regfiledump regfiletype sigcensus tricensus trisetcmp DESTINATION bin)
