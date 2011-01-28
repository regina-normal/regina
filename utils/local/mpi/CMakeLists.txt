
set(CMAKE_CXX_COMPILER ${MPI_COMPILER})
include_directories(${ENGINE_INCLUDES} ${MPI_INCLUDE_PATH})
set(UTILS distinguish_mpi equiv_mpi normal_compare_mpi normal_mpi sig_normal_mpi tricensus_freq_mpi tricensus_normal_mpi)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${MPI_LIBRARY} ${MPI_LIBRARIES} ${POPT_LIBRARY})
endforeach(UTIL)


extra_dist(FILES README.txt)
extra_dist(TARGETS normal-mpi-sanity tricensus-sanity-pairs-coverage tricensus-sanity-pairs-done tricensus-sanity-tricount)
