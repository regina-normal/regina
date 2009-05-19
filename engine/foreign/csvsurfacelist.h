
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

/*! \file foreign/csvsurfacelist.h
 *  \brief Exports normal surface lists to plain text CSV files.
 */

#ifndef __CSVSURFACELIST_H
#ifndef __DOXYGEN
#define __CSVSURFACELIST_H
#endif

#include <iostream>

namespace regina {

class NNormalSurfaceList;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Indicates a set of additional fields that can be exported as part
 * of a normal surface list.  This type can be used as a bitmask: to
 * describe a set of fields, simply combine several individual fields
 * using bitwise \e or.
 *
 * The list of available fields may grow with future releases of
 * Regina.
 */
enum SurfaceExportFields {
    surfaceExportName = 0x0001,
        /**< Represents the user-assigned surface name. */
    surfaceExportEuler = 0x0002,
        /**< Represents the calculated Euler characteristic of a
             surface.  This will be an integer, and will be left empty
             if the Euler characteristic cannot be computed. */
    surfaceExportOrient = 0x0004,
        /**< Represents the calculated property of whether a surface is
             orientable.  This will be \c TRUE or \c FALSE, or will be
             left empty if the orientability cannot be computed. */
    surfaceExportSides = 0x0008,
        /**< Represents the calculated property of whether a surface is
             one-sided or two-sided.  This will be the integer 1 or 2,
             or will be left empty if the "sidedness" cannot be computed. */
    surfaceExportBdry = 0x0010,
        /**< Represents the calculated property of whether a surface is
             bounded.  This will be one of the strings "closed",
             "real bdry" or "infinite" (where "infinite" indicates a
             surface with infinitely many discs). */
    surfaceExportLink = 0x0020,
        /**< Represents whether a surface is a single vertex link or a
             thin edge link.  See NNormalSurface::isVertexLink() and
             NNormalSurface::isThinEdgeLink() for details.  This will be
             written as a human-readable string. */
    surfaceExportType = 0x0040,
        /**< Represents any additional high-level properties of a
             surface, such as whether it is a splitting surface or a
             central surface.  This will be written as a human-readable
             string.  This field is somewhat arbitrary, and the precise
             properties it describes are subject to change in future
             releases of Regina. */

    surfaceExportNone = 0,
        /**< Indicates that no additional fields should be exported. */
    surfaceExportAllButName = 0x007e,
        /**< Indicates that all available fields should be exported,
             except for the user-assigned surface name.  Since the list
             of available fields may grow with future releases, the numerical
             value of this constant may change as a result. */
    surfaceExportAll = 0x007f
        /**< Indicates that all available fields should be exported,
             including the user-assigned surface name.  Since the list
             of available fields may grow with future releases, the numerical
             value of this constant may change as a result. */
};

/**
 * Exports the given list of normal surfaces as a plain text CSV
 * (comma-separated value) file.  CSV files are human-readable and
 * human-editable, and are suitable for importing into spreadsheets and
 * databases.
 *
 * The surfaces will be exported in standard coordinates (tri-quad
 * coordinates for normal surfaces, or tri-quad-oct coordinates for
 * almost normal surfaces).  Each coordinate will become a separate
 * field in the CSV file.
 *
 * As well as the normal surface coordinates, additional properties of the
 * normal surfaces (such as Euler characteristic, orientability, and so on)
 * can be included as extra fields in the export.  Users can select
 * precisely which properties to include by passing a bitmask formed
 * from regina::SurfaceExportFields constants.
 *
 * The CSV format used here begins with a header row, and uses commas as
 * field separators.  Text fields with arbitrary contents are placed inside
 * double quotes, and the double quote character itself is represented by a
 * pair of double quotes.  Thus the string
 * <tt>my "normal" surface's name</tt> would be stored as
 * <tt>"my ""normal"" surface's name"</tt>.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O
 * routines.  Any user strings such as surface names will be written
 * in UTF-8.
 *
 * @param filename the name of the CSV file to export to.
 * @param surfaces the list of normal surfaces to export.
 * @param additionalFields a bitwise combination of regina::SurfaceExportFields
 * constants indicating which additional properties of surfaces should
 * be included in the export.
 * @return \c true if the export was successful, or \c false otherwise.
 */
bool writeCSVStandard(const char* filename, NNormalSurfaceList& surfaces,
    int additionalFields = surfaceExportAll);

/**
 * Exports the given list of normal surfaces as a plain text CSV
 * (comma-separated value) file.  CSV files are human-readable and
 * human-editable, and are suitable for importing into spreadsheets and
 * databases.
 *
 * The surfaces will be exported in edge weight coordinates.  Thus
 * there will be one coordinate for each edge of the underlying
 * triangulation; each such coordinate will become a separate field in
 * the CSV file.
 *
 * As well as the normal surface coordinates, additional properties of the
 * normal surfaces (such as Euler characteristic, orientability, and so on)
 * can be included as extra fields in the export.  Users can select
 * precisely which properties to include by passing a bitmask formed
 * from regina::SurfaceExportFields constants.
 *
 * The CSV format used here begins with a header row, and uses commas as
 * field separators.  Text fields with arbitrary contents are placed inside
 * double quotes, and the double quote character itself is represented by a
 * pair of double quotes.  Thus the string
 * <tt>my "normal" surface's name</tt> would be stored as
 * <tt>"my ""normal"" surface's name"</tt>.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O
 * routines.  Any user strings such as surface names will be written
 * in UTF-8.
 *
 * @param filename the name of the CSV file to export to.
 * @param surfaces the list of normal surfaces to export.
 * @param additionalFields a bitwise combination of regina::SurfaceExportFields
 * constants indicating which additional properties of surfaces should
 * be included in the export.
 * @return \c true if the export was successful, or \c false otherwise.
 */
bool writeCSVEdgeWeight(const char* filename, NNormalSurfaceList& surfaces,
    int additionalFields = surfaceExportAll);

/*@}*/

} // namespace regina

#endif

