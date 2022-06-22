# Try to find an appropriate key-value store library.
#
# Sets the following variables:
#    KVStore_FOUND - system has a usable key-value store library
#    KVStore_INCLUDE_DIRS - the include directories for this library
#    KVStore_LIBRARIES - libraries needed to link to use this library
#    KVStore_DISPLAYNAME - the human-readable name of the pre-selected library
#    KVStore_DISPLAYURL - the human-readable URL for the pre-selected library
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


SET(KVStore_OPTIONS "any tokyocabinet qdbm lmdb")

IF (QDBM)
  MESSAGE(FATAL_ERROR
    "The old QDBM setting has been removed. Please set REGINA_KVSTORE=qdbm instead.")
ENDIF (QDBM)

IF (NOT REGINA_KVSTORE)
  SET (REGINA_KVSTORE any CACHE STRING
    "Choose which key-value store Regina will use for its census databases.  Options are: ${KVStore_OPTIONS}."
    FORCE)
ENDIF (NOT REGINA_KVSTORE)

SET(KVStore_DISPLAYNAME "Tokyo Cabinet / QDBM / LMDB")
SET(KVStore_DISPLAYURL "http://fallabs.com/tokyocabinet/")

SET(REGINA_KVSTORE_OPTION_FOUND FALSE)

IF (${REGINA_KVSTORE} STREQUAL tokyocabinet OR REGINA_KVSTORE_TOKYOCABINET OR
    (${REGINA_KVSTORE} STREQUAL any AND NOT KVStore_FOUND))
  SET(REGINA_KVSTORE_OPTION_FOUND TRUE)
  PKG_CHECK_MODULES(KVStore tokyocabinet)
  IF (NOT KVStore_FOUND)
    # Either pkg-config is missing or it could not find the library.
    find_path(KVStore_INCLUDE_DIRS NAMES tcbdb.h)
    find_library(KVStore_LIBRARIES NAMES tokyocabinet libtokyocabinet)
    IF (KVStore_INCLUDE_DIRS AND KVStore_LIBRARIES)
      SET(KVStore_FOUND TRUE CACHE INTERNAL "")
    ENDIF (KVStore_INCLUDE_DIRS AND KVStore_LIBRARIES)
  ENDIF (NOT KVStore_FOUND)

  IF (${REGINA_KVSTORE} STREQUAL tokyocabinet OR KVStore_FOUND)
    SET(REGINA_KVSTORE_TOKYOCABINET TRUE CACHE INTERNAL "")
    SET(KVStore_DISPLAYNAME "Tokyo Cabinet")
    SET(KVStore_DISPLAYURL "http://fallabs.com/tokyocabinet/")
    SET(DB_EXT "tdb")
  ENDIF (${REGINA_KVSTORE} STREQUAL tokyocabinet OR KVStore_FOUND)
ENDIF()

IF (${REGINA_KVSTORE} STREQUAL qdbm OR REGINA_KVSTORE_QDBM OR
    (${REGINA_KVSTORE} STREQUAL any AND NOT KVStore_FOUND))
  SET(REGINA_KVSTORE_OPTION_FOUND TRUE)
  PKG_CHECK_MODULES(KVStore qdbm)
  IF (NOT KVStore_FOUND)
    # Either pkg-config is missing or it could not find the library.
    find_path(KVStore_INCLUDE_DIRS NAMES cabin.h)
    find_library(KVStore_LIBRARIES NAMES qdbm libqdbm)
    IF (KVStore_INCLUDE_DIRS AND KVStore_LIBRARIES)
      SET(KVStore_FOUND TRUE CACHE INTERNAL "")
    ENDIF (KVStore_INCLUDE_DIRS AND KVStore_LIBRARIES)
  ENDIF (NOT KVStore_FOUND)

  IF (${REGINA_KVSTORE} STREQUAL qdbm OR KVStore_FOUND)
    SET(REGINA_KVSTORE_QDBM TRUE CACHE INTERNAL "")
    SET(KVStore_DISPLAYNAME "QDBM")
    SET(KVStore_DISPLAYURL "http://fallabs.com/qdbm/")
    SET(DB_EXT "qdb")
  ENDIF (${REGINA_KVSTORE} STREQUAL qdbm OR KVStore_FOUND)
ENDIF()

IF (${REGINA_KVSTORE} STREQUAL lmdb OR REGINA_KVSTORE_LMDB OR
    (${REGINA_KVSTORE} STREQUAL any AND NOT KVStore_FOUND))
  SET(REGINA_KVSTORE_OPTION_FOUND TRUE)
  PKG_CHECK_MODULES(KVStore lmdb)
  IF (NOT KVStore_FOUND)
    # Either pkg-config is missing or it could not find the library.
    find_path(KVStore_INCLUDE_DIRS NAMES lmdb.h)
    find_library(KVStore_LIBRARIES NAMES lmdb liblmdb)
    IF (KVStore_INCLUDE_DIRS AND KVStore_LIBRARIES)
      SET(KVStore_FOUND TRUE CACHE INTERNAL "")
    ENDIF (KVStore_INCLUDE_DIRS AND KVStore_LIBRARIES)
  ENDIF (NOT KVStore_FOUND)

  IF (${REGINA_KVSTORE} STREQUAL lmdb OR KVStore_FOUND)
    SET(REGINA_KVSTORE_LMDB TRUE CACHE INTERNAL "")
    SET(KVStore_DISPLAYNAME "LMDB")
    SET(KVStore_DISPLAYURL "https://symas.com/mdb/")
    SET(DB_EXT "lmdb")
  ENDIF (${REGINA_KVSTORE} STREQUAL lmdb OR KVStore_FOUND)
ENDIF()

IF ((NOT REGINA_KVSTORE_OPTION_FOUND) AND (NOT KVStore_FOUND))
  MESSAGE(FATAL_ERROR "REGINA_KVSTORE must be one of: ${KVStore_OPTIONS}")
ENDIF ((NOT REGINA_KVSTORE_OPTION_FOUND) AND (NOT KVStore_FOUND))

mark_as_advanced(KVStore_INCLUDE_DIRS KVStore_LIBRARIES)

