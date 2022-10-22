
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file reginaqt.h
 *  \brief Provides Qt configuration for Regina.  This must be included
 *  in all of Regina's GUI sources, _before_ any Qt headers.
 */

#ifndef __REGINAQT_H
#define __REGINAQT_H

// If we are building against Qt6, then explicitly disable everything
// from Qt5 that is now deprecated in Qt6.
//
// A problem: we should do this *before* including QtGlobal, since QtGlobal
// will define QT_DISABLE_DEPRECATED_BEFORE if we have not already done so.
//
// For this, we test for the existence of a header that is new to the Qt6 API.

#if __has_include(<QStringConverter>)
    // We are building against Qt6 (or later).
    #define QT_DISABLE_DEPRECATED_BEFORE 0x060000
#else
    // We are building against Qt5.
    // Just to be sure, check that our __has_include is checking the
    // Qt header directory at all.
    #if ! __has_include(<QString>)
        #error Cannot find Qt headers!
    #endif
#endif

#endif

