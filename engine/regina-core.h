
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

namespace regina {

/**
 * Indicates whether the given dimension is one of Regina's
 * \ref stddim "standard dimensions".
 * Standard dimensions offer significantly richer functionality for
 * triangulations than generic dimensions.
 *
 * @param dim the dimension in question.
 * @return \c true if and only if \a dim is one of Regina's standard dimensions.
 */
constexpr bool standardDim(int dim) {
    return (dim == 2 || dim == 3 || dim == 4);
}

} // namespace regina

/**
 * A synonym for \c inline, used in some special cases to avoid
 * noisy warnings under Windows.
 *
 * Typically this macro is used with inline functions of some class or struct
 * that are called by \e other inline function(s).
 *
 * Such a function should:
 *
 * - be declared as \c REGINA_INLINE_REQUIRED at the point where the function
 *   is first declared inside the class definition;
 *
 * - be declared again as \c inline when the function is later defined
 *   (as Regina does with all its inline functions).
 *
 * The reason for using REGINA_INLINE_REQUIRED in such situations is to
 * avoid noisy warnings about \c dllimport attributes under Windows.
 * The reason for using this particular macro instead of just the
 * keyword \c inline is to remind the Regina developers not to remove it.
 *
 * This is not the first, and will surely not be the last, piece of code
 * bloat that Windows forces upon us.  Sigh.
 */
#define REGINA_INLINE_REQUIRED inline

/**
 * Any deprecated class, function or typedef in Regina's API must be
 * declared with REGINA_DEPRECATED.
 *
 * Where possible, this expands to an attribute that the compiler can
 * recognise (such as the C++14 attribute <tt>[[deprecated]]</tt>, which
 * some compilers already support for C++11).
 *
 * If a function is declared as both REGINA_API and REGINA_DEPRECATED, the
 * REGINA_DEPRECATED attribute should be listed first.
 *
 * Conversely, if a class is declared as both REGINA_API and REGINA_DEPRECATED,
 * the REGINA_API attribute should be listed first.
 */
#define REGINA_DEPRECATED [[deprecated]]

#ifdef __DOXYGEN
  // Fake definitions just for doxygen.

  /**
   * All non-templated, non-static functions, classes and global variables
   * that are part of Regina's public interface \b must be declared with
   * REGINA_API.  Template classes or functions should not be declared
   * with REGINA_API, but any explicit instantiations should.
   * Global variables must also be declared with \c extern, as per normal.
   *
   * This is required for DLL linking under Windows, and also to avoid
   * breakage with optimisations such as gcc's <tt>-fvisibility=hidden</tt>.
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
