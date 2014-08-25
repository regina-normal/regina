include_directories(${ENGINE_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/engine)
include_directories(${POPT_INCLUDE_DIR})

# The tokyocabinet library is used by the mkcensusdb utility, and so
# we must include the relevant search paths here also.
INCLUDE_DIRECTORIES(${TOKYOCABINET_INCLUDE_DIRS})
LINK_DIRECTORIES(${TOKYOCABINET_LIBRARY_DIRS})

set(UTILS
  censuslookup
  regconcat
  regconvert
  regfiledump
  regfiletype
  sigcensus
  tricensus
  trisetcmp
  )

set(PRIVATE_UTILS
  mkcensusdb
  )

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${POPT_LIBRARY})
endforeach(UTIL)

foreach(UTIL ${PRIVATE_UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${POPT_LIBRARY})
endforeach(UTIL)

target_link_libraries(mkcensusdb ${TOKYOCABINET_LIBRARIES})

if(MPI_FOUND)
  add_subdirectory(mpi)
endif(MPI_FOUND)

install(TARGETS ${UTILS} DESTINATION ${BINDIR} COMPONENT Runtime)
