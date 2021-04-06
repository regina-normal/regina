# Try to find an appropriate key-value store library.
#
# Sets the following variables:
#    KVSTORE_FOUND - system has a usable key-value store library
#    KVSTORE_INCLUDE_DIRS - the include directories for this library
#    KVSTORE_LIBRARIES - libraries needed to link to use this library
#    KVSTORE_DISPLAYNAME - the human-readable name of the pre-selected library
#    KVSTORE_DISPLAYURL - the human-readable URL for the pre-selected library
#
# Also sets exactly one of the following booleans to true, according to
# which library was configured and found:
#    REGINA_KVSTORE_TOKYOCABINET
#    REGINA_KVSTORE_QDBM
#    REGINA_KVSTORE_LMDB
#
# When adding a new key-value store library to the list of options, ensure:
# - the library supports reading from multiple processes (e.g., not LevelDB);
# - the library supports duplicate keys (e.g., not Kyoto Cabinet).
#
# Copyright (c) 2021, Ben Burton <bab@debian.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


SET(KVSTORE_OPTIONS "any tokyocabinet qdbm lmdb")

IF (QDBM)
  MESSAGE(FATAL_ERROR
    "The old QDBM setting has been removed. Please set REGINA_KVSTORE=qdbm instead.")
ENDIF (QDBM)

IF (NOT REGINA_KVSTORE)
  SET (REGINA_KVSTORE any CACHE STRING
    "Choose which key-value store Regina will use for its census databases.  Options are: ${KVSTORE_OPTIONS}."
    FORCE)
ENDIF (NOT REGINA_KVSTORE)

SET(KVSTORE_DISPLAYNAME "Tokyo Cabinet / QDBM / LMDB")
SET(KVSTORE_DISPLAYURL "http://fallabs.com/tokyocabinet/")

SET(REGINA_KVSTORE_OPTION_FOUND FALSE)

IF (${REGINA_KVSTORE} STREQUAL tokyocabinet OR REGINA_KVSTORE_TOKYOCABINET OR
    (${REGINA_KVSTORE} STREQUAL any AND NOT KVSTORE_FOUND))
  SET(REGINA_KVSTORE_OPTION_FOUND TRUE)
  PKG_CHECK_MODULES(KVSTORE tokyocabinet)
  IF (NOT KVSTORE_FOUND)
    # Either pkg-config is missing or it could not find the library.
    find_path(KVSTORE_INCLUDE_DIRS NAMES tcbdb.h)
    find_library(KVSTORE_LIBRARIES NAMES tokyocabinet libtokyocabinet)
    IF (KVSTORE_INCLUDE_DIRS AND KVSTORE_LIBRARIES)
      SET(KVSTORE_FOUND TRUE CACHE INTERNAL "")
    ENDIF (KVSTORE_INCLUDE_DIRS AND KVSTORE_LIBRARIES)
  ENDIF (NOT KVSTORE_FOUND)

  IF (${REGINA_KVSTORE} STREQUAL tokyocabinet OR KVSTORE_FOUND)
    SET(REGINA_KVSTORE_TOKYOCABINET TRUE CACHE INTERNAL "")
    SET(KVSTORE_DISPLAYNAME "Tokyo Cabinet")
    SET(KVSTORE_DISPLAYURL "http://fallabs.com/tokyocabinet/")
    SET(DB_EXT "tdb")
  ENDIF (${REGINA_KVSTORE} STREQUAL tokyocabinet OR KVSTORE_FOUND)
ENDIF()

IF (${REGINA_KVSTORE} STREQUAL qdbm OR REGINA_KVSTORE_QDBM OR
    (${REGINA_KVSTORE} STREQUAL any AND NOT KVSTORE_FOUND))
  SET(REGINA_KVSTORE_OPTION_FOUND TRUE)
  PKG_CHECK_MODULES(KVSTORE qdbm)
  IF (NOT KVSTORE_FOUND)
    # Either pkg-config is missing or it could not find the library.
    find_path(KVSTORE_INCLUDE_DIRS NAMES cabin.h)
    find_library(KVSTORE_LIBRARIES NAMES qdbm libqdbm)
    IF (KVSTORE_INCLUDE_DIRS AND KVSTORE_LIBRARIES)
      SET(KVSTORE_FOUND TRUE CACHE INTERNAL "")
    ENDIF (KVSTORE_INCLUDE_DIRS AND KVSTORE_LIBRARIES)
  ENDIF (NOT KVSTORE_FOUND)

  IF (${REGINA_KVSTORE} STREQUAL qdbm OR KVSTORE_FOUND)
    SET(REGINA_KVSTORE_QDBM TRUE CACHE INTERNAL "")
    SET(KVSTORE_DISPLAYNAME "QDBM")
    SET(KVSTORE_DISPLAYURL "http://fallabs.com/qdbm/")
    SET(DB_EXT "qdb")
  ENDIF (${REGINA_KVSTORE} STREQUAL qdbm OR KVSTORE_FOUND)
ENDIF()

IF (${REGINA_KVSTORE} STREQUAL lmdb OR REGINA_KVSTORE_LMDB OR
    (${REGINA_KVSTORE} STREQUAL any AND NOT KVSTORE_FOUND))
  SET(REGINA_KVSTORE_OPTION_FOUND TRUE)
  PKG_CHECK_MODULES(KVSTORE lmdb)
  IF (NOT KVSTORE_FOUND)
    # Either pkg-config is missing or it could not find the library.
    find_path(KVSTORE_INCLUDE_DIRS NAMES lmdb.h)
    find_library(KVSTORE_LIBRARIES NAMES lmdb liblmdb)
    IF (KVSTORE_INCLUDE_DIRS AND KVSTORE_LIBRARIES)
      SET(KVSTORE_FOUND TRUE CACHE INTERNAL "")
    ENDIF (KVSTORE_INCLUDE_DIRS AND KVSTORE_LIBRARIES)
  ENDIF (NOT KVSTORE_FOUND)

  IF (${REGINA_KVSTORE} STREQUAL lmdb OR KVSTORE_FOUND)
    SET(REGINA_KVSTORE_LMDB TRUE CACHE INTERNAL "")
    SET(KVSTORE_DISPLAYNAME "LMDB")
    SET(KVSTORE_DISPLAYURL "https://symas.com/mdb/")
    SET(DB_EXT "lmdb")
  ENDIF (${REGINA_KVSTORE} STREQUAL lmdb OR KVSTORE_FOUND)
ENDIF()

IF ((NOT REGINA_KVSTORE_OPTION_FOUND) AND (NOT KVSTORE_FOUND))
  MESSAGE(FATAL_ERROR "REGINA_KVSTORE must be one of: ${KVSTORE_OPTIONS}")
ENDIF ((NOT REGINA_KVSTORE_OPTION_FOUND) AND (NOT KVSTORE_FOUND))

mark_as_advanced(KVSTORE_INCLUDE_DIRS KVSTORE_LIBRARIES)

