set(CMAKE_CXX_COMPILER ${MPI_COMPILER})
include_directories(${ENGINE_INCLUDES})

set(UTILS tricensus-mpi)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${MPI_LIBRARY} ${MPI_LIBRARIES} ${POPT_LIBRARY})
endforeach(UTIL)

install(PROGRAMS tricensus-mpi-status DESTINATION ${BINDIR} COMPONENT Runtime)
install(TARGETS tricensus-mpi DESTINATION ${BINDIR} COMPONENT Runtime)
