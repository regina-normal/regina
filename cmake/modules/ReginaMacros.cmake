# CMake macros specific to Regina.
#
# Copyright (c) Ben Burton, 2012-2025
# Licensed under the GNU General Public License, version 2 or later
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.


# Macro: CHECK_GETOPT
#
# Sets the boolean variable GETOPT_FOUND according to whether the GNU libc
# function getopt_long is available via the header getopt.h.
#
macro (CHECK_GETOPT)
  SET (_SRCFILE "${CMAKE_SOURCE_DIR}/cmake/modules/getopt.cpp")
  SET (_BINDIR "${CMAKE_BINARY_DIR}/cmake/modules")

  MESSAGE(STATUS "Checking for GNU getopt_long()")
  try_compile(_REGINA_GETOPT ${_BINDIR} ${_SRCFILE})
  IF (_REGINA_GETOPT)
    SET (GETOPT_FOUND TRUE)
    MESSAGE(STATUS "Checking for GNU getopt_long() -- found")
  ELSE (_REGINA_GETOPT)
    SET (GETOPT_FOUND FALSE)
    MESSAGE(STATUS "Checking for GNU getopt_long() -- not found")
  ENDIF (_REGINA_GETOPT)
endmacro (CHECK_GETOPT)


# Macro: CHECK_LSB_RELEASE
#
# Sets the string variables LSB_RELEASE_ID and LSB_RELEASE_VERSION, if
# this information can be deduced from the system lsb_release command.
#
# Otherwise these variables will be unset.
#
macro (CHECK_LSB_RELEASE)
  find_program(LSB_RELEASE lsb_release)
  if (LSB_RELEASE)
    execute_process(COMMAND "${LSB_RELEASE}" -is
      OUTPUT_VARIABLE LSB_RELEASE_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND "${LSB_RELEASE}" -rs
      OUTPUT_VARIABLE LSB_RELEASE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    MESSAGE(STATUS "Checking Linux distribution -- ${LSB_RELEASE_ID} ${LSB_RELEASE_VERSION}")
  else (LSB_RELEASE)
    MESSAGE(STATUS "Checking Linux distribution -- not available")
    UNSET(LSB_RELEASE_ID)
    UNSET(LSB_RELEASE_VERSION)
  endif (LSB_RELEASE)
endmacro (CHECK_LSB_RELEASE)


# Macro: REGINA_ESCAPE_BASH(input)
#
# Sets the variable BASH_${input} to be a variant of the variable ${input}
# that is properly escaped for use in a bash script between single quotes.
#
# The input argument must be a variable name (not its value).
#
# This macro correctly handles characters that are special to cmake
# as well as characters that are special to bash.
#
macro (REGINA_ESCAPE_BASH _input)
  string(REPLACE "'" "'\"'\"'" BASH_${_input} "${${_input}}")
endmacro (REGINA_ESCAPE_BASH)


# Macro: REGINA_ESCAPE_BASH_BARE(input)
#
# Sets the variable BASH_BARE_${input} to be a variant of the variable ${input}
# that is properly escaped for use in a bash script in an unquoted context.
#
# The input argument must be a variable name (not its value).
#
# This macro correctly handles characters that are special to cmake
# as well as characters that are special to bash.
#
macro (REGINA_ESCAPE_BASH_BARE _input)
  string(REPLACE "\\" "\\\\" BASH_BARE_${_input} "${${_input}}")
  string(REGEX REPLACE "([#'\"`$*?;&!(){}<>[|~ \t])" "\\\\\\1"
    BASH_BARE_${_input} "${BASH_BARE_${_input}}")
  string(REPLACE "]" "\\]" BASH_BARE_${_input} "${BASH_BARE_${_input}}")
endmacro (REGINA_ESCAPE_BASH_BARE)


# Macro: REGINA_ESCAPE_PERL(input)
#
# Sets the variable PERL_${input} to be a variant of the variable ${input}
# that is properly escaped for use in a perl script between single quotes.
#
# The input argument must be a variable name (not its value).
#
# This macro correctly handles characters that are special to perl
# as well as characters that are special to bash.
#
macro (REGINA_ESCAPE_PERL _input)
  string(REPLACE "\\" "\\\\" PERL_${_input} "${${_input}}")
  string(REPLACE "'" "\\'" PERL_${_input} "${PERL_${_input}}")
endmacro (REGINA_ESCAPE_PERL)


# Macro: REGINA_ESCAPE_MAKE(input)
#
# Sets the variable MAKE_${input} to be a variant of the variable ${input}
# that is properly escaped for use in a Makefile rule.  This means that
# (i) it will be escaped for use in bash in an _unquoted_ context, and then
# (ii) any dollar signs will be replaced with double dollar signs.
#
# As an exception, newlines will NOT be escaped.
#
# The input argument must be a variable name (not its value).
#
macro (REGINA_ESCAPE_MAKE _input)
  string(REPLACE "\\" "\\\\" MAKE_${_input} "${${_input}}")
  string(REGEX REPLACE "([#'\"`$*?;&!(){}<>[|~ \t])" "\\\\\\1"
    MAKE_${_input} "${MAKE_${_input}}")
  string(REPLACE "]" "\\]" MAKE_${_input} "${MAKE_${_input}}")
  string(REPLACE "$" "$$" MAKE_${_input} "${MAKE_${_input}}")
endmacro (REGINA_ESCAPE_MAKE)


# Macro: REGINA_ESCAPE_C(input)
#
# Sets the variable C_${input} to be a variant of the variable ${input}
# that is properly escaped for use in a C string between double quotes.
#
# Also sets the variable C_QQ_${input} to be the same, but without escaping
# double-quote characters.  This is intended for scenarios where double-quotes
# (but only double-quotes) have already been escaped (such as headers created
# via cmake's CONFIGURE_FILE(... ESCAPE_QUOTES).
#
# The input argument must be a variable name (not its value).
#
macro (REGINA_ESCAPE_C _input)
  string(REPLACE "\\" "\\\\" C_QQ_${_input} "${${_input}}")
  string(REPLACE "\"" "\\\"" C_${_input} "${C_QQ_${_input}}")
endmacro (REGINA_ESCAPE_C)


# Macro: REGINA_ESCAPE_URI_PATH(input)
#
# Sets the path variable URI_${input} to be a variant of the variable ${input}
# that is properly percent-escaped for use as a URI.
#
# Since ${input} is assumed to represent a path, all reserved URI characters
# except for the forward slash will also be percent-escaped.
#
# The input argument must be a variable name (not its value).
#
macro (REGINA_ESCAPE_URI_PATH _input)
  set(URI_${_input} "")

  # The following character-by-character extraction breaks in two ways:
  # - if the string contains a semicolon then this is lost, because cmake
  #   uses semicolons internally to separate the full *list* of matches;
  # - if the string contains a backslash then this is incorrectly combined
  #   with the following character.
  #
  # We work around this by doing our own custom encoding first, and
  # removing this at the end.
  #
  string(REPLACE "_" "_u" _intermediate "${${_input}}")
  string(REPLACE ";" "_s" _intermediate "${_intermediate}")
  string(REPLACE "\\" "_b" _intermediate "${_intermediate}")

  string(REGEX MATCHALL . chars "${_intermediate}")
  foreach(c ${chars})
    string(REGEX MATCH "^[A-Za-z0-9_.~/-]$" m "${c}")
    if (NOT m STREQUAL "")
      # These characters are allowed and unreserved in URIs
      # (except for / which is reserved but which we also preserve here).
      string(APPEND URI_${_input} "${c}")
    else (NOT m STREQUAL "")
      # All other characters must be percent-encoded.
      # Note that these may be multi-byte unicode characters.
      #
      # Whilst CMake 3.18 introduces hex encoding for strings,
      # we must preserve compatibility with older cmake, and so we
      # use file read/writes to get the hex encoding.  Again I'd be
      # super happy for a better way of doing this.
      file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/_hex.dat "${c}")
      file(READ ${CMAKE_CURRENT_BINARY_DIR}/_hex.dat _hexraw HEX)

      string(REGEX MATCHALL "(..)" _hexlist "${_hexraw}")
      list(JOIN _hexlist "%" _hex)
      string(PREPEND _hex "%")
      string(TOUPPER "${_hex}" _hexupper)
      string(APPEND URI_${_input} "${_hexupper}")
    endif (NOT m STREQUAL "")
  endforeach()

  string(REPLACE "_s" ";" URI_${_input} "${URI_${_input}}")
  string(REPLACE "_b" "%5C" URI_${_input} "${URI_${_input}}")
  string(REPLACE "_u" "_" URI_${_input} "${URI_${_input}}")
endmacro (REGINA_ESCAPE_URI_PATH)


# Macro: REGINA_CREATE_HANDBOOK(lang)
#
# Builds an HTML manual from XML Docbook sources.
#
# Includes code from KDE4_CREATE_HANDBOOK from the KDE cmake scripts.
# The KDE cmake scripts are licensed as follows:
#
# Copyright (c) 2006-2009 Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2006, 2007, Laurent Montel, <montel@kde.org>
# Copyright (c) 2007 Matthias Kretz <kretz@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
macro (REGINA_CREATE_HANDBOOK _lang)
  # Build the docbook documentation.

  set(_doc "${CMAKE_CURRENT_BINARY_DIR}/index.html")

  # The source variables below are relative to ${CMAKE_CURRENT_SOURCE_DIR}.
  # We use relative paths here because xsltproc on Windows seems to have
  # trouble with the format that cmake uses for absolute paths.
  #
  set(_input index.docbook)
  set(_custom ../ksgmltools2/customization)
  set(_dtd ${_custom}/dtd)
  set(_ssheet ${_custom}/regina.xsl)

  get_filename_component(_handbook ${CMAKE_CURRENT_SOURCE_DIR} NAME)

  file(GLOB _docs *.docbook)
  if (REGINA_DOCS)
    add_custom_command(OUTPUT "${_doc}" VERBATIM
      COMMAND "${UNZIP_EXECUTABLE}" -o -j -d "${CMAKE_CURRENT_BINARY_DIR}"
        "${REGINA_DOCS_FILE}" "docs/${_lang}/${_handbook}/\\*")
  else (REGINA_DOCS)
    # xsltproc requires an URI-encoded output directory.
    REGINA_ESCAPE_URI_PATH(CMAKE_CURRENT_BINARY_DIR)
    add_custom_command(OUTPUT "${_doc}" VERBATIM
      COMMAND "${XSLTPROC_EXECUTABLE}" --path ${_dtd} -o "${URI_CMAKE_CURRENT_BINARY_DIR}/" ${_ssheet} "${_input}"
      DEPENDS ${_docs} ${_ssheet}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endif (REGINA_DOCS)
  add_custom_target(${_handbook}-html ALL DEPENDS "${_doc}")

  file(GLOB _support *.png *.css *.html)
  install(FILES ${_support} DESTINATION "${PKGHTMLDIR}/${_lang}/${_handbook}")
  install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DESTINATION "${PKGHTMLDIR}/${_lang}"
    FILES_MATCHING
    PATTERN CMakeFiles EXCLUDE
    PATTERN "*.html"
    )
endmacro (REGINA_CREATE_HANDBOOK)

