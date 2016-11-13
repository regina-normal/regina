
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#ifndef __GLOBALDIRS_H
#ifndef __DOXYGEN
#define __GLOBALDIRS_H
#endif

#include <string>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup file
 * @{
 */

/**
 * Provides global routines that return directories in which various
 * components of Regina are installed on the system.
 *
 * By default, these routines are only useful with a fixed filesystem
 * installation of Regina (e.g., a typical Linux install).  Specifically,
 * they return the relevant directories as they were configured by \e cmake
 * at build time.
 *
 * If Regina may have been moved around on the filesystem (e.g., if you are
 * running an app bundle on MacOS), then you \e must call setDirs() when your
 * application starts.  Otherwise the directories that GlobalDirs returns
 * might be incorrect, and might not even exist.
 *
 * The directories returned by home(), pythonModule() and census() can be
 * independently changed at runtime, by calling setDirs().  All other
 * directories maintained by this class are subdirectories of home().
 *
 * At present this class does not support running Regina directly out of
 * the source tree.  This might be supported in future versions of Regina.
 */
class REGINA_API GlobalDirs {
    public:
        /**
         * Returns Regina's primary home directory on the system.  This
         * directory should contains subdirectories \e data, \e icons/,
         * \e examples/ and so on.
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina .
         *
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
         *
         * @return Regina's primary home directory.
         */
        static std::string home();

        /**
         * Returns the directory in which Regina's python module is installed,
         * or the empty string if the module is installed in python's
         * standard site-packages directory.
         *
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
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
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
         *
         * @return Regina's calculation engine data directory.
         */
        static std::string census();

        /**
         * Returns the directory in which optional "helper" Python libraries
         * are installed.  These libraries are not a formal part of Regina,
         * but can be made to load automatically as extra user libraries
         * through Regina's python settings.
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/pylib .
         *
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
         *
         * @return Regina's optional Python library directory.
         */
        static std::string pythonLibs();

        /**
         * Returns the directory in which example data files (including
         * the smaller but human-browsable census data files) are installed.
         *
         * This is computed automatically as a subdirectory of home().
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/examples .
         *
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
         *
         * @return Regina's example and census data directory.
         */
        static std::string examples();

        /**
         * Returns the directory in which API documentation for Regina's
         * calculation engine is installed.
         *
         * This is computed automatically as a subdirectory of home().
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/engine-docs .
         *
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
         *
         * @return Regina's calculation engine documentation directory.
         */
        static std::string engineDocs();

        /**
         * Returns the directory containing miscellaneous data files
         * for internal use Regina's calculation engine.
         *
         * This is computed automatically as a subdirectory of home().
         *
         * On a typical GNU/Linux system, this directory might (for example) be
         * \c /usr/local/share/regina/data .
         *
         * \warning If Regina is not installed in the exact location
         * configured at compile time (e.g., if you are running a MacOSX
         * app bundle), you \e must call setDirs() before calling this routine.
         * \warning If you are running out of the source tree, this
         * routine will almost certainly return an incorrect (and possibly
         * non-existent) directory.
         *
         * @return Regina's calculation engine data directory.
         */
        static std::string data();

        /**
         * Tells Regina where data files are installed.  You must call this
         * at runtime if Regina is not installed in the location that was
         * configured by \e cmake at build time (e.g., if you are
         * running a MacOSX app bundle).
         *
         * Empty strings are treated as follows:
         *
         * - If \a homeDir or \a censusDir is an empty string, then the
         *   corresponding directory will not be changed.
         *
         * - If \a pythonDir is an empty string then this has an explicit
         *   meaning, namely that the python module has been installed in
         *   python's standard site-packages directory.
         *
         * @param homeDir Regina's primary home directory; this will be
         * returned by homeDir().
         * @param pythonModuleDir the directory containing Regina's
         * python module, or the empty string if the module has been
         * installed in python's standard site-packages directory;
         * this will be returned by pythonModule().
         * @param censusDir The directory containing the large
         * machine-encoded census data files; this will be returned
         * by census().
         */
        static void setDirs(const std::string& homeDir,
                const std::string& pythonModuleDir,
                const std::string& censusDir = std::string());

    private:
        static std::string home_;
            /**< Regina's primary home directory. */
        static std::string pythonModule_;
            /**< The directory containing Regina's python module. */
        static std::string census_;
            /**< The directory containing the large machine-encoded
                 census data files. */
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NGlobalDirs has now been renamed to GlobalDirs.
 */
REGINA_DEPRECATED typedef GlobalDirs NGlobalDirs;

/*@}*/

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

inline std::string GlobalDirs::pythonLibs() {
    return home_ + "/pylib";
}

inline std::string GlobalDirs::examples() {
    return home_ + "/examples";
}

inline std::string GlobalDirs::engineDocs() {
    return home_ + "/engine-docs";
}

inline std::string GlobalDirs::data() {
    return home_ + "/data";
}



} // namespace regina

#endif

