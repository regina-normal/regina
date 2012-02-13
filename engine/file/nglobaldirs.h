
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

/*! \file file/nglobaldirs.h
 *  \brief Gives information about system installation directories.
 */

#ifndef __NGLOBALDIRS_H
#ifndef __DOXYGEN
#define __NGLOBALDIRS_H
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
 * running an app bundle on MacOS), you \e must call setDirs() when your
 * application starts.  Otherwise the directories that NGlobalDirs might
 * be incorrect, and might not even exist.
 *
 * At present this class does not support running Regina directly out of
 * the source tree.  This might be supported in future versions of Regina.
 */
class REGINA_API NGlobalDirs {
    public:
        /**
         * Returns Regina's primary home directory on the system.  This
         * directory should contains subdirectories \e scripts/, \e icons/,
         * \e examples/ and so on.
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
         * Returns the directory in which Regina's python module is installed.
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
         * Returns the directory in which optional "helper" Python libraries
         * are installed.  These libraries are not a formal part of Regina,
         * but can be made to load automatically as extra user libraries
         * through Regina's python settings.
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
         * Returns the directory in which example data files and census
         * data files are installed.
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
         * Returns the directory containing internal data files for Regina's
         * calculation engine.
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
         * Tells Regina where data files are installed.  This is
         * necessary if Regina is not installed in the location that was
         * configured by \e cmake at build time (e.g., if you are
         * running a MacOSX app bundle).
         *
         * @param homeDir Regina's primary home directory, which will be
         * returned by homeDir().
         * @param pythonModuleDir the directory containing Regina's
         * python module, which will be returned by pythonModule().
         */
        static void setDirs(const std::string& homeDir,
                const std::string& pythonModuleDir);

    private:
        static std::string home_;
            /**< Regina's primary home directory. */
        static std::string pythonModule_;
            /**< The directory containing Regina's python module. */
};

/*@}*/

} // namespace regina

#endif

