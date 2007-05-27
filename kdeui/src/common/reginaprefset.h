
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
 * Describes the many possible ways in which ReginaPrefSet::triGraphvizExec
 * might or might not reflect a usable Graphviz installation on the current
 * machine.
 */
class GraphvizStatus {
    public:
        /**
         * Indicates that the current Graphviz status is unknown.
         */
        static const GraphvizStatus unknown;
        /**
         * Indicates that ReginaPrefSet::triGraphvizExec describes an
         * executable with no path information that could not be found
         * on the current system.
         */
        static const GraphvizStatus notFound;
        /**
         * Indicates that ReginaPrefSet::triGraphvizExec describes an
         * executable, possibly including path information, that does
         * not exist on the current system.
         */
        static const GraphvizStatus notExist;
        /**
         * Indicates that ReginaPrefSet::triGraphvizExec describes a
         * program that exists but is not marked as executable on the
         * current system.
         */
        static const GraphvizStatus notExecutable;
        /**
         * Indicates that ReginaPrefSet::triGraphvizExec describes an
         * executable that could not be started.
         */
        static const GraphvizStatus notStartable;
        /**
         * Indicates that Graphviz has been found on the current system,
         * but its version is unsupported by Regina.
         */
        static const GraphvizStatus unsupported;
        /**
         * Indicates that Graphviz version 1.x has been found on the
         * current system.
         */
        static const GraphvizStatus version1;
        /**
         * Indicates that Graphviz version 2.x or later has been found
         * on the current system.
         */
        static const GraphvizStatus version2;

    private:
        int flag_;
            /**< A constant that distinguishes between the different
                 status types. */

    public:
        /**
         * Constructors.  The default constructor sets this status to
         * GraphvizStatus::unknown.
         */
        GraphvizStatus();
        GraphvizStatus(const GraphvizStatus& status);

        GraphvizStatus& operator = (const GraphvizStatus& status);
        bool operator == (const GraphvizStatus& status) const;
        bool operator != (const GraphvizStatus& status) const;

        /**
         * Is it known that the Graphviz cannot be used on the
         * current machine?
         */
        bool unusable() const;
        /**
         * Is it known that the Graphviz can be used on the
         * current machine?
         */
        bool usable() const;

    private:
        GraphvizStatus(int flag);
};

/**
 * A structure holding all Regina preferences.
 */
struct ReginaPrefSet {
    enum TriEditMode { DirectEdit, Dialog };
        /**< Possible edit modes for triangulation gluings. */
    enum TriTab { Gluings, Skeleton, Algebra, Composition, Surfaces, SnapPea };
        /**< Available top-level tabs in a triangulation viewer/editor. */
    enum TriSkeletonTab { SkelComp, FacePairingGraph };
        /**< Available tabs in a triangulation skeleton viewer. */
    enum TriAlgebraTab { Homology, FundGroup, TuraevViro, CellularInfo };
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
    bool snapPeaClosed;
        /**< Do we allow the SnapPea kernel to work with closed (non-ideal)
             triangulations, even though it's dangerous? */
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
    QString triGraphvizExec;
        /**< The executable for starting Graphviz.  This should be a tool
             for drawing undirected graphs; the recommended Graphviz tool
             is neato.  This need not include a directory (in which case
             the search path will be used). */
    QString triGraphvizExecFull;
        /**< The full pathname to the Graphviz executable \a triGraphvizExec
             if known, or QString::null if not known.  This is derived at
             runtime, and is not read from or written to the configuration
             file. */
    GraphvizStatus triGraphvizStatus;
        /**< The status of the Graphviz installation on the current
             machine.  This is derived at runtime, and is not read from
             or written to the configuration file. */
    TriTab triInitialTab;
        /**< The initially visible top-level tab for a new triangulation
             viewer/editor. */
    TriSkeletonTab triInitialSkeletonTab;
        /**< The initially visible tab for a new triangulation skeleton
             viewer. */
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

    /**
     * Updates the known status of the Graphviz installation on the current
     * machine.  Members \a triGraphvizStatus and \a triGraphvizExecFull
     * are set by this routine.  By default, the status will not be
     * rechecked if \a triGraphvizStatus is not GraphvizStatus::unknown.
     */
    void checkGraphvizStatus(bool forceRecheck = false);
};

inline GraphvizStatus::GraphvizStatus() : flag_(unknown.flag_) {
}

inline GraphvizStatus::GraphvizStatus(const GraphvizStatus& status) :
        flag_(status.flag_) {
}

inline GraphvizStatus::GraphvizStatus(int flag) : flag_(flag) {
}

inline GraphvizStatus& GraphvizStatus::operator = (
        const GraphvizStatus& status) {
    flag_ = status.flag_;
    return *this;
}

inline bool GraphvizStatus::operator == (const GraphvizStatus& status) const {
    return (flag_ == status.flag_);
}

inline bool GraphvizStatus::operator != (const GraphvizStatus& status) const {
    return (flag_ != status.flag_);
}

inline bool GraphvizStatus::unusable() const {
    return (*this == notFound || *this == notExist ||
        *this == notExecutable || *this == notStartable ||
        *this == unsupported);
}

inline bool GraphvizStatus::usable() const {
    return (*this == version1 || *this == version2);
}

inline ReginaFilePref::ReginaFilePref() : active(true) {
}

inline ReginaFilePref::ReginaFilePref(const QString& newFilename,
        bool newActive) : filename(newFilename), active(newActive) {
}

#endif

