# Copyright (c) William Pettersson, 2011 
# Licensed under the GNU General Public License, version 2 or later
find_path(POPT_INCLUDE_DIR popt.h PATHS /usr/include /usr/local/lib)
find_library(POPT_LIBRARY popt PATHS /usr/lib /usr/local/lib)


IF(POPT_INCLUDE_DIR AND POPT_LIBRARY)
  SET(POPT_FOUND TRUE)
ENDIF(POPT_INCLUDE_DIR AND POPT_LIBRARY)

IF(POPT_FOUND)
  IF (NOT POPT_FIND_QUIETLY)
    MESSAGE(STATUS "Found popt: ${POPT_LIBRARY}")
  ENDIF (NOT POPT_FIND_QUIETLY)
ELSE (POPT_FOUND)
  IF (POPT_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find the popt library.")
  ENDIF (POPT_FIND_REQUIRED)
ENDIF(POPT_FOUND)
