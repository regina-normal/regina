# Modified from FindGMP.cmake. - Ben Burton, 11 November 2011.

# Try to find the GMPXX librairies
#  GMPXX_FOUND - system has GMPXX lib
#  GMPXX_INCLUDE_DIR - the GMPXX include directory
#  GMPXX_LIBRARIES - Libraries needed to use GMPXX

# The original FindGMP.cmake is copyright (c) 2006, Laurent Montel,
# <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (GMPXX_INCLUDE_DIR AND GMPXX_LIBRARIES)
  # Already in cache, be silent
  set(GMPXX_FIND_QUIETLY TRUE)
endif (GMPXX_INCLUDE_DIR AND GMPXX_LIBRARIES)

find_path(GMPXX_INCLUDE_DIR NAMES gmpxx.h )
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMPXX DEFAULT_MSG GMPXX_INCLUDE_DIR GMPXX_LIBRARIES)

mark_as_advanced(GMPXX_INCLUDE_DIR GMPXX_LIBRARIES)
