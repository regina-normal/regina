set(CMAKE_CXX_COMPILER ${MPI_COMPILER})
include_directories(${ENGINE_INCLUDES})

set(UTILS tricensus-mpi)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE "/_/-/" ${UTIL} SOURCE_FILE)
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${MPI_LIBRARY} ${MPI_LIBRARIES})
endforeach(UTIL)


set(EXTRA README.txt tricensus-mpi-status)
foreach(E ${EXTRA}
  set(EXTRA_D ${CMAKE_CURRENT_SOURCE_DIR}/${E})
endforeach(E)

set(EXTRA_DIST ${EXTRA_DIST} ${EXTRA_D} PARENT_SCOPE)

install(PROGRAMS tricensus-mpi-status DESTINATION bin)
install(TARGETS tricensus-mpi DESTINATION bin)
