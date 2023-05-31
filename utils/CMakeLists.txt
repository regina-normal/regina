include_directories(${ENGINE_INCLUDES})
include_directories("${CMAKE_BINARY_DIR}/engine")

set(UTILS
  censuslookup
  dgt
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
  add_executable(${UTIL} ${SOURCE_FILE}.cpp)
  target_link_libraries(${UTIL} ${ENGINE_LIBRARY})
endforeach(UTIL)

install(TARGETS ${UTILS} DESTINATION "${BINDIR}" COMPONENT Runtime)

ADD_SUBDIRECTORY("testsuite")
