# Modified from FindGMP.cmake. - Ben Burton, 24 November 2016.

# Try to find the Jansson librairies
#  Jansson_FOUND - system has jansson lib
#  Jansson_INCLUDE_DIR - the jansson include directory
#  Jansson_LIBRARIES - Libraries needed to use jansson

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (Jansson_INCLUDE_DIR AND Jansson_LIBRARIES)
  # Already in cache, be silent
  set(Jansson_FIND_QUIETLY TRUE)
endif (Jansson_INCLUDE_DIR AND Jansson_LIBRARIES)

find_path(Jansson_INCLUDE_DIR NAMES jansson.h )
find_library(Jansson_LIBRARIES NAMES jansson libjansson)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Jansson DEFAULT_MSG Jansson_INCLUDE_DIR Jansson_LIBRARIES)

mark_as_advanced(Jansson_INCLUDE_DIR Jansson_LIBRARIES)
