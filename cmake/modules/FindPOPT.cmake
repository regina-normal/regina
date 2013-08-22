# Copyright (c) William Pettersson, 2011 
# Licensed under the GNU General Public License, version 2 or later
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.

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
