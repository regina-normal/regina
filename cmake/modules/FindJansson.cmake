# Modified from FindGMP.cmake. - Ben Burton, 24 November 2016.

# Try to find the Jansson librairies
#  JANSSON_FOUND - system has jansson lib
#  JANSSON_INCLUDE_DIR - the jansson include directory
#  JANSSON_LIBRARIES - Libraries needed to use jansson

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (JANSSON_INCLUDE_DIR AND JANSSON_LIBRARIES)
  # Already in cache, be silent
  set(JANSSON_FIND_QUIETLY TRUE)
endif (JANSSON_INCLUDE_DIR AND JANSSON_LIBRARIES)

find_path(JANSSON_INCLUDE_DIR NAMES jansson.h )
find_library(JANSSON_LIBRARIES NAMES jansson libjansson)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JANSSON DEFAULT_MSG JANSSON_INCLUDE_DIR JANSSON_LIBRARIES)

mark_as_advanced(JANSSON_INCLUDE_DIR JANSSON_LIBRARIES)
