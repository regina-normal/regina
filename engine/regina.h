
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file regina.h
 *  \brief Platform-independent global definition file.
 *  Items should be the same across all
 *  platforms, but may differ between release versions.
 */

#ifndef __REGINA_H
#define __REGINA_H

#define PROGRAM_NAME "Regina"
    /**< String name of the program, to be placed at the beginning of
     *   data files. */
#define ENGINE_VERSION_MAJOR 2
    /**< Major version number of the engine. */
#define ENGINE_VERSION_MINOR 1
    /**< Minor version number of the engine. */
#define ENGINE_VERSION "2.1.1"
    /**< Complete version number of the engine. */
#define SIZE_INT 4
    /**< The number of bytes written to files to represent an integer;
     *   any higher order bytes will be ignored.  This ensures
     *   constancy of file format across platforms. */
#define SIZE_LONG 8
    /**< The number of bytes written to files to represent a long integer;
     *   any higher order bytes will be ignored.  This ensures
     *   constancy of file format across platforms. */
#define SIZE_FILEPOS 8
    /**< The number of bytes written to files to represent a file position;
     *   any higher order bytes will be ignored.  This ensures
     *   constancy of file format across platforms. */
    
#endif

