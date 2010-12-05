
set(CMAKE_CXX_COMPILER ${MPI_COMPILER})
include_directories(${ENGINE_INCLUDES} ${MPI_INCLUDE_PATH})
set(UTILS distinguish_mpi equiv_mpi normal_compare_mpi normal_mpi tricensus_freq_mpi tricensus_normal_mpi)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE "/_/-/" ${UTIL} SOURCE_FILE)
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${MPI_LIBRARY} ${MPI_LIBRARIES})
endforeach(UTIL)


set(EXTRA README.txt normal-mpi-sanity tricensus-sanity-pairs-coverage tricensus-sanity-pairs-done tricensus-sanity-tricount)
foreach(E ${EXTRA}
  set(EXTRA_D ${CMAKE_CURRENT_SOURCE_DIR}/${E})
endforeach(E)

set(EXTRA_DIST ${EXTRA_DIST} ${EXTRA_D} PARENT_SCOPE)
