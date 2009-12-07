
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

namespace regina {

/**
 * \weakgroup file
 * @{
 */

/**
 * Provides global routines that return directories in which various
 * components of Regina are installed on the system.
 *
 * These routines are only useful with a normal installation of Regina.
 * Specifically, they return the relevant directories as they \e should be
 * according to the arguments given to the \e configure script at build time.
 * If Regina is being run directly out of the source tree, or if files were
 * moved around after Regina was installed, then these directories might well
 * not exist.
 *
 * It is possible that, in future versions of Regina, these routines will
 * support more interesting scenarios, such as running out of the source
 * tree or moving ready-made "application bundles" around the system.
 */
class NGlobalDirs {
    public:
        /**
         * Returns Regina's primary home directory on the system.  This
         * directory should contains subdirectories \e scripts/, \e icons/,
         * \e examples/ and so on.
         *
         * \warning Currently this routine only returns useful results
         * if Regina has been properly installed on the system.  See the
         * NGlobalDirs class notes for further details.
         *
         * @return Regina's primary home directory.
         */
        static std::string home();

        /**
         * Returns the directory in which Regina's python module is installed.
         *
         * \warning Currently this routine only returns useful results
         * if Regina has been properly installed on the system.  See the
         * NGlobalDirs class notes for further details.
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
         * \warning Currently this routine only returns useful results
         * if Regina has been properly installed on the system.  See the
         * NGlobalDirs class notes for further details.
         *
         * @return Regina's optional Python library directory.
         */
        static std::string pythonLibs();

        /**
         * Returns the directory in which example data files and census
         * data files are installed.
         *
         * \warning Currently this routine only returns useful results
         * if Regina has been properly installed on the system.  See the
         * NGlobalDirs class notes for further details.
         *
         * @return Regina's example and census data directory.
         */
        static std::string examples();

        /**
         * Returns the directory in which API documentation for Regina's
         * calculation engine is installed.
         *
         * \warning Currently this routine only returns useful results
         * if Regina has been properly installed on the system.  See the
         * NGlobalDirs class notes for further details.
         *
         * @return Regina's calculation engine documentation directory.
         */
        static std::string engineDocs();

        /**
         * Returns the directory containing internal data files for Regina's
         * calculation engine.
         *
         * \warning Currently this routine only returns useful results
         * if Regina has been properly installed on the system.  See the
         * NGlobalDirs class notes for further details.
         *
         * @return Regina's calculation engine data directory.
         */
        static std::string data();
};

/*@}*/

} // namespace regina

#endif

