
include_directories(${ENGINE_INCLUDES})

set(UTILS censuscat dehydrate edgelinks equiv identify manifolds normal normal_compare pluggedibundles reduce trisort volume)
foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  include_directories(${POPT_INCLUDE_DIR})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} popt)
endforeach(UTIL)

if(MPI_FOUND)
  add_subdirectory(mpi)
endif(MPI_FOUND)

extra_dist(FILES README.txt)
