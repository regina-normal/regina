
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

/*! \file nsnappea.h
 *  \brief Provides a mechanism for reading SnapPea files.
 */

#ifndef __NSNAPPEA_H
#ifndef __DOXYGEN
#define __NSNAPPEA_H
#endif

class NTriangulation;

/**
 * Reads a triangulation from the given SnapPea file.  A newly allocated
 * triangulation will be returned; it is the user's responsibility to
 * deallocate this when it is finished with.
 *
 * If the file could not be read or if the data was not in the correct
 * format, 0 will be returned.
 *
 * \pre The first two lines of the SnapPea file each contain at most
 * 1000 characters.  The first line is the type of file which should
 * simply be ``<tt>% Triangulation</tt>''.  The second line is the name of
 * the manifold.
 *
 * \todo \feature Allow exporting of SnapPea files also.
 *
 * @param filename the name of the SnapPea file from which to read.
 * @return a new triangulation containing the data read from the SnapPea
 * file, or 0 on error.
 */
NTriangulation* readSnapPea(const char *filename);

#endif

