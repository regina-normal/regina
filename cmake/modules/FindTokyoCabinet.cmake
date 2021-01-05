# Modified from FindGMP.cmake. - Ben Burton, 14 July 2020.

# Try to find the TokyoCabinet librairy, or equivalent
#  TOKYOCABINET_FOUND - system has Tokyo Cabinet or equivalent
#  TOKYOCABINET_INCLUDE_DIRS - the Tokyo Cabinet include directory
#  TOKYOCABINET_LIBRARIES - Libraries needed to use Tokyo Cabinet

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
# Copyright (c) 2020-2021, Ben Burton <bab@debian.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


IF(NOT QDBM)
  SET (QDBM OFF CACHE STRING
    "Makes Regina build against the older QDBM library, instead of the newer Tokyo Cabinet.  This should only be used on systems (e.g., MinGW) to which Tokyo Cabinet cannot be reliably ported."
    FORCE)
ENDIF(NOT QDBM)
IF (QDBM)
  SET (QDBM_AS_TOKYOCABINET TRUE)
  SET (TOKYOCABINET_LIBNAME qdbm)
  SET (TOKYOCABINET_DISPLAYNAME "QDBM")
  SET (TOKYOCABINET_DISPLAYURL "http://fallabs.com/qdbm/")
  SET (TOKYOCABINET_TESTHEADER cabin.h)
  SET (DB_EXT "qdb")
ELSE (QDBM)
  SET (QDBM_AS_TOKYOCABINET FALSE)
  SET (TOKYOCABINET_LIBNAME tokyocabinet)
  SET (TOKYOCABINET_DISPLAYNAME "Tokyo Cabinet")
  SET (TOKYOCABINET_DISPLAYURL "http://fallabs.com/tokyocabinet/")
  SET (TOKYOCABINET_TESTHEADER tcbdb.h)
  SET (DB_EXT "tdb")
ENDIF (QDBM)

PKG_CHECK_MODULES(TOKYOCABINET ${TOKYOCABINET_LIBNAME})

IF (NOT TOKYOCABINET_FOUND)
  # This includes the case where pkg-config could not find tokyocabinet,
  # and also the case where pkg-config itself could not be found.

  if (TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)
    # Already in cache, be silent
    set(TOKYOCABINET_FIND_QUIETLY TRUE)
  endif (TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)

  find_path(TOKYOCABINET_INCLUDE_DIRS NAMES ${TOKYOCABINET_TESTHEADER})
  find_library(TOKYOCABINET_LIBRARIES NAMES ${TOKYOCABINET_LIBNAME} lib${TOKYOCABINET_LIBNAME})

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(TokyoCabinet DEFAULT_MSG TOKYOCABINET_INCLUDE_DIRS TOKYOCABINET_LIBRARIES)

  mark_as_advanced(TOKYOCABINET_INCLUDE_DIRS TOKYOCABINET_LIBRARIES)
ENDIF (NOT TOKYOCABINET_FOUND)

