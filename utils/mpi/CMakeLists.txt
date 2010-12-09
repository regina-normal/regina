set(CMAKE_CXX_COMPILER ${MPI_COMPILER})
include_directories(${ENGINE_INCLUDES})

set(UTILS tricensus-mpi)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${MPI_LIBRARY} ${MPI_LIBRARIES})
endforeach(UTIL)


extra_dist(FILES README.txt)

install(PROGRAMS tricensus-mpi-status DESTINATION bin)
install(TARGETS tricensus-mpi DESTINATION bin)
