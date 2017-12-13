# Some utilities need to know about the installation type.
IF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  ADD_DEFINITIONS(-DREGINA_INSTALL_BUNDLE=1)
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Windows)
  ADD_DEFINITIONS(-DREGINA_INSTALL_WINDOWS=1)
ENDIF ()

include_directories(${ENGINE_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/engine)
include_directories(${POPT_INCLUDE_DIR})

set(UTILS
  censuslookup
  regconcat
  regconvert
  regfiledump
  regfiletype
  retriangulate
  sigcensus
  tricensus
  trisetcmp
  )

foreach(UTIL ${UTILS})
  string(REGEX REPLACE _ - SOURCE_FILE ${UTIL})
  add_executable(${UTIL} ${SOURCE_FILE})
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY} ${POPT_LIBRARY})
endforeach(UTIL)

if(MPI_FOUND)
  add_subdirectory(mpi)
endif(MPI_FOUND)

install(TARGETS ${UTILS} DESTINATION ${BINDIR} COMPONENT Runtime)
