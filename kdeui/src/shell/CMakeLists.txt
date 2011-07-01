SET(SOURCES examplesaction.cpp main.cpp reginamain.cpp reginapref.cpp)
AUTOMOC4_MOC_HEADERS(regina-kde examplesaction.h reginamain.h reginapref.h)
KDE4_ADD_EXECUTABLE(regina-kde ${COMMON_SOURCES} ${SOURCES})

TARGET_LINK_LIBRARIES(regina-kde ${ENGINE_LIBRARY})
