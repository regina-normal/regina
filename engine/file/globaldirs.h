
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file file/globaldirs.h
 *  \brief Gives information about system installation directories.
 */

#ifndef __REGINA_GLOBALDIRS_H
#ifndef __DOXYGEN
#define __REGINA_GLOBALDIRS_H
#endif

#include <string>
#include "regina-core.h"

namespace regina {

/**
 * Provides global routines that return directories in which various
 * components of Regina are installed on the system.
 *
 * By default, these routines return directories that were configured by
 * \e cmake when Regina was compiled, which will only be useful with a
 * fixed filesystem installation of Regina (e.g., a typical Linux install).
 *
 * If Regina may be running from a different place in the filesystem (e.g.,
 * if you are running an app bundle on macOS or if you are running directly
 * from the source tree), then you \e must call either setDirs() or deduceDirs()
 * when your application starts.  Otherwise the directories that GlobalDirs
 * returns might be incorrect, and might not even exist.
 *
 * \ingroup file
 */
class GlobalDirs {
    public:
        /**
         * Returns Regina's primary home directory on the system.  This
         * directory should contains subdirectories \e data, \e icons/,
         * \e examples/ and so on.
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina .
         *
         * \warning If Regina is not installed in the exact location configured
         * at compile time (e.g., if you are running a macOS app bundle or you
         * are running directly out of the source tre), you \e must call
         * either setDirs() or deduceDirs() before calling this routine.
         *
         * @return Regina's primary home directory.
         */
        static std::string home();

        /**
         * Returns the directory in which Regina's python module is installed,
         * or the empty string if the module is installed in python's
         * standard site-packages directory.
         *
         * \warning If Regina is not installed in the exact location configured
         * at compile time (e.g., if you are running a macOS app bundle or you
         * are running directly out of the source tre), you \e must call
         * either setDirs() or deduceDirs() before calling this routine.
         *
         * @return Regina's python module directory.
         */
        static std::string pythonModule();

        /**
         * Returns the directory containing the large machine-encoded
         * census data files.  These data files are not human-browsable:
         * instead they are built for fast performance and small size.
         * Users can access them by calling Census::lookup().
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/data/census .
         *
         * \warning If Regina is not installed in the exact location configured
         * at compile time (e.g., if you are running a macOS app bundle or you
         * are running directly out of the source tre), you \e must call
         * either setDirs() or deduceDirs() before calling this routine.
         *
         * @return Regina's calculation engine data directory.
         */
        static std::string census();

        /**
         * Returns the directory in which example data files (including
         * the smaller but human-browsable census data files) are installed.
         *
         * This is computed automatically as the <tt>examples/</tt> subdirectory
         * of home().
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/examples .
         *
         * \warning If Regina is not installed in the exact location configured
         * at compile time (e.g., if you are running a macOS app bundle or you
         * are running directly out of the source tre), you \e must call
         * either setDirs() or deduceDirs() before calling this routine.
         *
         * @return Regina's example and census data directory.
         */
        static std::string examples();

        /**
         * Returns the directory in which API documentation for Regina's
         * calculation engine is installed.
         *
         * This is computed automatically:
         *
         * - in most cases it will be the <tt>engine-docs/</tt> subdirectory
         *   of home();
         *
         * - in the special case where deduceDirs() was called and we are
         *   running from the build tree, it will be the location in the
         *   build tree where the API docs are built.
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/engine-docs .
         *
         * \warning If Regina is not installed in the exact location configured
         * at compile time (e.g., if you are running a macOS app bundle or you
         * are running directly out of the source tre), you \e must call
         * either setDirs() or deduceDirs() before calling this routine.
         *
         * @return Regina's calculation engine documentation directory.
         */
        static std::string engineDocs();

        /**
         * Returns the directory containing miscellaneous data files
         * for internal use Regina's calculation engine.
         *
         * This is computed automatically as the <tt>data/</tt> subdirectory
         * of home().
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/data .
         *
         * \warning If Regina is not installed in the exact location configured
         * at compile time (e.g., if you are running a macOS app bundle or you
         * are running directly out of the source tre), you \e must call
         * either setDirs() or deduceDirs() before calling this routine.
         *
         * @return Regina's calculation engine data directory.
         */
        static std::string data();

        /**
         * Tells Regina explicitly where its supporting files are installed.
         *
         * You should call either setDirs() or deduceDirs() at runtime if
         * Regina is not installed in the location that was configured by
         * \e cmake at build time (e.g., if you are running a macOS app bundle,
         * or if you are running directly out of the source tree).
         *
         * In most settings, it is better to call deduceDirs(), since
         * this avoids you (the programmer) having to manage through the
         * many different possible platforms and runtime environments.
         *
         * Empty strings are treated as follows:
         *
         * - If \a homeDir or \a censusDir is an empty string, then the
         *   corresponding directory will not be changed.  Instead it will
         *   left at its previous value from the last call to setDirs()
         *   or deduceDirs(), or at the build-time configured default if
         *   neither setDirs() nor deduceDirs() has been called before.
         *
         * - If \a pythonDir is an empty string then this has an explicit
         *   meaning, namely that the python module has been installed in
         *   python's standard site-packages directory.
         *
         * @param homeDir Regina's primary home directory; this will be
         * returned by homeDir().
         * @param pythonDir the directory containing Regina's
         * python module, or the empty string if the module has been
         * installed in python's standard site-packages directory;
         * this will be returned by pythonModule().
         * @param censusDir The directory containing the large
         * machine-encoded census data files; this will be returned
         * by census().
         */
        static void setDirs(const std::string& homeDir,
                const std::string& pythonDir,
                const std::string& censusDir = std::string());

        /**
         * Ask Regina to deduce where its supporting files are installed.
         *
         * You should call either setDirs() or deduceDirs() at runtime if
         * Regina is not installed in the location that was configured by
         * \e cmake at build time (e.g., if you are running a macOS app bundle,
         * or if you are running directly out of the source tree).
         *
         * This routine sets all of the relevant paths automatically, based on
         * the type of build and the location of the given executable (which
         * is typically the program currently being run).
         * It can identify the correct paths for fixed XDG installations,
         * relocatable macOS bundles, Windows installations, and also
         * running directly from within the source tree.
         *
         * If running from within the source tree, the executable must be
         * inside the \e build tree (any depth of subdirectory within the
         * build tree is fine).  Moreover, it is assumed that the build
         * tree is either an immediate subdirectory of the source root,
         * or else is the source root itself.  So, for example, an
         * executable path of <tt>regina-X.Y/qtui/src/regina-gui</tt>
         * or <tt>regina-X.Y/build/qtui/src/regina-gui</tt> is fine,
         * but <tt>regina-X.Y/tmp/build/qtui/src/regina-gui</tt> is not.
         *
         * This routine respects the following environment variables,
         * and these will take precedence over any automatic deductions:
         *
         * - REGINA_HOME, if present and non-empty, will determine Regina's
         *   primary home directory as returned by homeDir().  It will also
         *   fix the census data directory as returned by censusDir() to be
         *   <tt>$REGINA_HOME/data/census</tt>.
         *
         * - REGINA_PYLIBDIR, if present and non-empty, will determine the
         *   directory containing Regina's python module as returned by
         *   pythonModule().
         *
         * If you have an unusual setup where Regina cannot deduce the paths
         * correctly, you can always call setDirs() to set the paths
         * explicitly yourself.
         *
         * @param executable the path to an executable, which would
         * typically be the executable currently being run.
         */
        static void deduceDirs(const char* executable);

        // Make this class non-constructible.
        GlobalDirs() = delete;

    private:
        static std::string home_;
            /**< Regina's primary home directory. */
        static std::string pythonModule_;
            /**< The directory containing Regina's python module. */
        static std::string census_;
            /**< The directory containing the large machine-encoded
                 census data files. */
        static std::string engineDocs_;
            /**< The directory containing Regina's API documentation. */
};

// Inline functions for GlobalDirs:

inline std::string GlobalDirs::home() {
    return home_;
}

inline std::string GlobalDirs::pythonModule() {
    return pythonModule_;
}

inline std::string GlobalDirs::census() {
    return census_;
}

inline std::string GlobalDirs::engineDocs() {
    return engineDocs_;
}

inline std::string GlobalDirs::examples() {
    return home_ + "/examples";
}

inline std::string GlobalDirs::data() {
    return home_ + "/data";
}

} // namespace regina

#endif

