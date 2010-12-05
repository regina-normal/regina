
include_directories(${ENGINE_INCLUDES} /usr/include/snappea)


add_executable(dehn1 dehn1)
target_link_libraries(dehn1 snappea ${ENGINE_LIBRARY})


set(EXTRA README.txt)

foreach(E ${EXTRA}
  set(EXTRA_D ${CMAKE_CURRENT_SOURCE_DIR}/${E})
endforeach(E)

set(EXTRA_DIST ${EXTRA_DIST} ${EXTRA_D} PARENT_SCOPE)

