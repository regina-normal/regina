
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file reginafilter.h
 *  \brief A variety of filename filters for use with KFileDialog.
 */

#ifndef __REGINAFILTER_H
#define __REGINAFILTER_H

/**
 * Filename filter for all supported files.
 */
#define FILTER_SUPPORTED \
    "*.rga|Regina Data Files\n*.py|Python Libraries\n*|All Files"

/**
 * Filename filter for Regina data files.
 */
#define FILTER_REGINA \
    "*.rga|Regina Data Files\n*|All Files"

/**
 * Filename filter for Python scripts.
 */
#define FILTER_PYTHON \
    "*.py|Python Libraries\n*|All Files"

/**
 * Filename filter for SnapPea files.
 */
#define FILTER_SNAPPEA \
    "*|SnapPea Files\n*|All Files"

/**
 * Filename filter for all files.
 */
#define FILTER_ALL \
    "*|All Files"

#endif

