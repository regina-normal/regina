
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

/*! \file reginaprefset.h
 *  \brief Provides a single data structure for all Regina preferences.
 */

#ifndef __REGINAPREFSET_H
#define __REGINAPREFSET_H

/**
 * A structure holding all Regina preferences.
 */
struct ReginaPrefSet {
    enum TriEditMode { DirectEdit, Dialog };
        /**< Possible edit modes for triangulation gluings. */

    bool autoDock;
        /**< Do we automatically dock new packet
             viewers into the parent window? */
    bool autoFileExtension;
        /**< Should filenames be given an automatic extension? */
    bool displayIcon;
        /**< Should we display the pretty Regina icon? */
    bool displayTagsInTree;
        /**< Should we display packet tags in the visual tree? */
    unsigned treeJumpSize;
        /**< The number of steps corresponding to a jump up or down in
             the packet tree. */
    TriEditMode triEditMode;
        /**< The default mode for editing triangulations. */
    unsigned triSurfacePropsThreshold;
        /**< The maximum number of tetrahedra for which surface-related
             properties of triangulations will be automatically
             calculated. */

    /**
     * Default constructor that provides a reasonable set of defaults.
     */
    ReginaPrefSet();
};

inline ReginaPrefSet::ReginaPrefSet() :
        autoDock(true),
        autoFileExtension(true),
        displayIcon(true),
        displayTagsInTree(false),
        treeJumpSize(10),
        triEditMode(DirectEdit),
        triSurfacePropsThreshold(6) {
}

#endif

