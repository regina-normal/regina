
include_directories(${ENGINE_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/engine)
include_directories(${POPT_INCLUDE_DIR})

set(UTILS regconcat regconvert regfiledump regfiletype sigcensus trisetcmp)

if (not WIN32)
  # Don't try to build multithreaded apps under Windows... for now.
  set(UTILS ${UTILS} tricensus)
endif (not WIN32)

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${POPT_LIBRARY})
endforeach(UTIL)

if(MPI_FOUND)
  add_subdirectory(mpi)
endif(MPI_FOUND)

install(TARGETS ${UTILS} DESTINATION ${BINDIR} COMPONENT Runtime)
