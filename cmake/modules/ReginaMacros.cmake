# CMake macros specific to Regina.
#
# Copyright (c) Ben Burton, 2012-2021
# Licensed under the GNU General Public License, version 2 or later
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.


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
  install(FILES ${_support} DESTINATION "${HTMLDIR}/${_lang}/${_handbook}")
  install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DESTINATION "${HTMLDIR}/${_lang}"
    FILES_MATCHING
    PATTERN CMakeFiles EXCLUDE
    PATTERN "*.html"
    )
endmacro (REGINA_CREATE_HANDBOOK)

