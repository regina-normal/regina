
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include <qvaluevector.h>

/**
 * A structure holding a single filename which may or may not be active
 * (such as a census file or a python library).
 */
struct ReginaFilePref {
    QString filename;
        /**< The full filename. */
    bool active;
        /**< Whether or not this filename is currently active. */

    /**
     * Constructors that make the filename active by default.
     */
    ReginaFilePref();
    ReginaFilePref(const QString& newFilename, bool newActive = true);
};

/**
 * A structure holding a list of filenames each of which may or may not
 * be active.
 */
typedef QValueVector<ReginaFilePref> ReginaFilePrefList;

/**
 * A structure holding all Regina preferences.
 */
struct ReginaPrefSet {
    enum TriEditMode { DirectEdit, Dialog };
        /**< Possible edit modes for triangulation gluings. */
    enum TriTab { Gluings, Skeleton, Algebra, Composition, Surfaces, SnapPea };
        /**< Available top-level tabs in a triangulation viewer/editor. */
    enum TriAlgebraTab { Homology, FundGroup, TuraevViro };
        /**< Available tabs in a triangulation algebra viewer. */

    bool autoDock;
        /**< Do we automatically dock new packet
             viewers into the parent window? */
    bool autoFileExtension;
        /**< Should filenames be given an automatic extension? */
    ReginaFilePrefList censusFiles;
        /**< The list of data files to use for census lookups. */
    bool displayIcon;
        /**< Should we display the pretty Regina icon? */
    bool displayTagsInTree;
        /**< Should we display packet tags in the visual tree? */
    bool pythonAutoIndent;
        /**< Should auto-indent be enabled in python consoles? */
    ReginaFilePrefList pythonLibraries;
        /**< The python libraries to load upon each session startup. */
    unsigned pythonSpacesPerTab;
        /**< The number of spaces to insert when <TAB> is pressed in a
             python console. */
    bool pythonWordWrap;
        /**< Should python consoles be word wrapped? */
    int surfacesCreationCoords;
        /**< The default coordinate system for normal surface creation. */
    unsigned treeJumpSize;
        /**< The number of steps corresponding to a jump up or down in
             the packet tree. */
    TriEditMode triEditMode;
        /**< The default mode for editing triangulations. */
    QString triGAPExec;
        /**< The executable for starting GAP.  This need not include a
             directory (in which case the search path will be used). */
    TriTab triInitialTab;
        /**< The initially visible top-level tab for a new triangulation
             viewer/editor. */
    TriAlgebraTab triInitialAlgebraTab;
        /**< The initially visible tab for a new triangulation algebra
             viewer. */
    unsigned triSurfacePropsThreshold;
        /**< The maximum number of tetrahedra for which surface-related
             properties of triangulations will be automatically
             calculated. */

    /**
     * Default constructor that provides a reasonable set of defaults.
     */
    ReginaPrefSet();

    /**
     * Returns the default census files shipped with Regina.
     */
    static ReginaFilePrefList defaultCensusFiles();

    /**
     * Returns the full path to the python libraries configuration file.
     */
    static QString pythonLibrariesConfig();

    /**
     * Reads the python libraries from the regina-python configuration
     * file.
     */
    bool readPythonLibraries();

    /**
     * Writes the python libraries to the regina-python configuration
     * file.
     */
    bool writePythonLibraries() const;
};

inline ReginaFilePref::ReginaFilePref() : active(true) {
}

inline ReginaFilePref::ReginaFilePref(const QString& newFilename,
        bool newActive) : filename(newFilename), active(newActive) {
}

#endif

