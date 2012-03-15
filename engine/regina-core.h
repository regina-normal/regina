
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file regina-core.h
 *  \brief Core definitions that must be included in every Regina header file.
 */

#ifndef __REGINA_CORE_H
#ifndef __DOXYGEN
#define __REGINA_CORE_H
#endif

/**
 * \weakgroup engine
 * @{
 */

#ifdef __DOXYGEN
  // Fake definitions just for doxygen.

  /**
   * All non-templated, non-static functions, classes and global variables
   * that are part of Regina's public interface \b must be declared with
   * REGINA_API.
   * In addition, global variables must also be declared with \c extern
   * as per normal.
   * Otherwise things may (and in some environments will) break when external
   * applications try to use Regina with optimisations such as gcc's
   * <tt>-fvisibility=hidden</tt>.
   *
   * Note:  When building the Regina calculation engine shared library,
   * REGINA_DLL_EXPORTS must be defined (this ensures that API symbols are
   * marked for export).  When importing (using) this library,
   * REGINA_DLL_EXPORTS must \e not be defined (this ensures that API symbols
   * are marked for import instead).
   */
  #define REGINA_API

  /**
   * Classes and functions that are local to the current compilation unit
   * and should not be publicly exported may be declared with REGINA_LOCAL.
   * Use of this macro is optional.
   */
  #define REGINA_LOCAL
#else
  // The real definitions go here.
  // The macros below are modified from the instructions at
  // http://gcc.gnu.org/wiki/Visibility (retrieved on 22 May 2011).
  #if defined _WIN32 || defined __CYGWIN__
    #define REGINA_HELPER_DLL_IMPORT __declspec(dllimport)
    #define REGINA_HELPER_DLL_EXPORT __declspec(dllexport)
    #define REGINA_HELPER_DLL_LOCAL
  #else
    #if __GNUC__ >= 4
      #define REGINA_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
      #define REGINA_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
      #define REGINA_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
    #else
      #define REGINA_HELPER_DLL_IMPORT
      #define REGINA_HELPER_DLL_EXPORT
      #define REGINA_HELPER_DLL_LOCAL
    #endif
  #endif

  // Assume that the library is always built as a shared library (not static).
  #ifdef REGINA_DLL_EXPORTS
    #define REGINA_API REGINA_HELPER_DLL_EXPORT
  #else
    #define REGINA_API REGINA_HELPER_DLL_IMPORT
  #endif
  #define REGINA_LOCAL REGINA_HELPER_DLL_LOCAL
#endif // doxygen

/*@}*/

#endif
