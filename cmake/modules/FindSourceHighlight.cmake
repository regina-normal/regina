# Copyright (c) William Pettersson and Ben Burton, 2011 
# Licensed under the GNU General Public License, version 2 or later
find_path(SRCHILITE_INCLUDE_DIR srchilite/sourcehighlight.h
  PATHS /usr/include /usr/local/lib)
find_library(SRCHILITE_LIBRARY source-highlight PATHS /usr/lib /usr/local/lib)

IF(SRCHILITE_INCLUDE_DIR AND SRCHILITE_LIBRARY)
  SET(SRCHILITE_FOUND TRUE)
ENDIF(SRCHILITE_INCLUDE_DIR AND SRCHILITE_LIBRARY)

IF(SRCHILITE_FOUND)
  IF (NOT SRCHILITE_FIND_QUIETLY)
    MESSAGE(STATUS "Found source-highlight: ${SRCHILITE_LIBRARY}")
  ENDIF (NOT SRCHILITE_FIND_QUIETLY)
ELSE (SRCHILITE_FOUND)
  IF (SRCHILITE_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find the source-highlight library.")
  ENDIF (SRCHILITE_FIND_REQUIRED)
ENDIF(SRCHILITE_FOUND)
