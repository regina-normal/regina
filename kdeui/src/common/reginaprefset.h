
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file reginaprefset.h
 *  \brief Provides a single data structure for all Regina preferences.
 */

#ifndef __REGINAPREFSET_H
#define __REGINAPREFSET_H

#include <qmutex.h>
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

    /**
     * Return the filename in an 8-bit encoding suitable for passing to
     * low-level file I/O routines.
     */
    QCString encodeFilename() const;
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
         * current system, and that dot appears to be selected.
         * Under Graphviz 1.x, dot seems to be the only tool that can
         * handle multiple edges correctly.
         */
        static const GraphvizStatus version1;
        /**
         * Indicates that Graphviz version 1.x has been found on the
         * current system, but that dot does not appear to be selected.
         * Under Graphviz 1.x, dot seems to be the only tool that can
         * handle multiple edges correctly.
         */
        static const GraphvizStatus version1NotDot;
        /**
         * Indicates that Graphviz version 2.x or later has been found
         * on the current system.
         */
        static const GraphvizStatus version2;

    private:
        int flag_;
            /**< A constant that distinguishes between the different
                 status types. */

    private:
        static QMutex cacheGraphvizMutex;
            /**< Protects the graphviz status cache, whose variables are
                 defined below. */

        // The Graphviz status cache.  This stores the results of the
        // last call to status(), so future calls with the same
        // Graphviz executable are fast.
        static QString cacheGraphvizExec;
            /**< The last user-provided executable that was tested.
                 This corresponds to ReginaPrefSet::triGraphvizExec,
                 and need not include full path details. */
        static QString cacheGraphvizExecFull;
            /**< The full pathname to the Graphviz executable corresponding
                 to \a cacheGraphvizExec, or QString::null if this full
                 pathname is unknown. */
        static GraphvizStatus cacheGraphvizStatus;
            /**< The status of the Graphviz installation corresponding
                 to \a cacheGraphvizExec. */

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

        /**
         * Determines the status of the Graphviz installation on this
         * machine corresponding to the given executable \a userExec.
         * This executable corresponds to ReginaPrefSet::triGraphvizExec,
         * and need not include full path details.
         *
         * The status is returned as the usual return value from this
         * routine, and the full pathname to the Graphviz executable is
         * returns in \a fullExec (this might be QString::null if the
         * Graphviz installation is not usable).
         *
         * Subsequent calls to this routine with the same \a userExec
         * argument are fast, since the last returned values are cached.
         * Calling with a different \a userExec will cause the cache to
         * be cleared (i.e., only one set of results is cached at a time).
         * The cache can be ignored by passing \a forceRecheck as \c true.
         *
         * This routine is thread-safe.
         */
         static GraphvizStatus status(const QString& userExec,
            QString& fullExec, bool forceRecheck = false);

    private:
        GraphvizStatus(int flag);
};

/**
 * A structure holding all Regina preferences.
 */
struct ReginaPrefSet {
    // Some defaults that other classes may need to access:
    static const char* defaultGAPExec;
        /**< The default setting for \a triGAPExec. */
    static const char* defaultGraphvizExec;
        /**< The default setting for \a triGraphvizExec. */

    // The preferences themselves:

    enum TriEditMode { DirectEdit, Dialog };
        /**< Possible edit modes for triangulation gluings. */
    enum TriTab { Gluings, Skeleton, Algebra, Composition, Surfaces, SnapPea };
        /**< Available top-level tabs in a triangulation viewer/editor. */
    enum TriSkeletonTab { SkelComp, FacePairingGraph };
        /**< Available tabs in a triangulation skeleton viewer. */
    enum TriAlgebraTab { Homology, FundGroup, TuraevViro, CellularInfo };
        /**< Available tabs in a triangulation algebra viewer. */
    enum SurfacesTab { Summary, Coordinates, Matching, Compatibility };
        /**< Available top-level tabs in a normal surface list viewer. */

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
    bool pdfAutoClose;
        /**< Should we close external PDF viewers automatically (e.g.,
             when the packet is refreshed or closed)? */
    bool pdfEmbed;
        /**< Should we view PDFs using an embedded KPart if possible? */
    QString pdfExternalViewer;
        /**< The external program used to view PDFs if we cannot find
             (or do not want) an embedded KPart.  This string may contain
             additional arguments for the viewer; the PDF filename will
             added to the end (separated by whitespace) and the entire
             string will be passed to a shell for interpretation.  If this is
             empty, Regina will do its best to find a suitable viewer. */
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
    SurfacesTab surfacesInitialTab;
        /**< The initially visible top-level tab for a new normal
             surface list viewer. */
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
     * Returns an external program suitable for viewing PDF files, or
     * the null string if none could be found.  This default is used
     * if the user preference \a pdfExternalViewer is left empty.
     *
     * The string returned will contain a program and possibly some
     * additional arguments.  It is expected that a PDF filename will be
     * added to the end of this string (separated by whitespace) and
     * that the entire string will be parsed by a shell.
     *
     * This routine does not query the services that offer themselves to KDE;
     * indeed, the generic KRun infrastructure will be used as a fallback if
     * this routine returns the null string.  The application returned
     * here is intended to have higher priority than what KDE knows about
     * (for instance, this routine refers to Preview.app on MacOS 10.x).
     */
    static QString pdfDefaultViewer();

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

