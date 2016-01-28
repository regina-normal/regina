
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file reginaprefset.h
 *  \brief Provides a single data structure for all Regina preferences.
 */

#ifndef __REGINAPREFSET_H
#define __REGINAPREFSET_H

#include "hypersurface/hypercoords.h"
#include "hypersurface/hyperflags.h"
#include "surfaces/normalcoords.h"
#include "surfaces/normalflags.h"
#include <qmutex.h>
#include <QFont>
#include <QList>
#include <QString>
#include <QSize>
#include <QUrl>

class QSettings;
class QTextCodec;
class QWidget;

/**
 * A structure holding a single filename which may or may not be active
 * (such as a census file or a python library).
 */
struct ReginaFilePref {
    private:
        QString filename_;
            /**< The full filename. */
        QString displayName_;
            /**< The long human-readable text to display for this file. */
        QString systemKey_;
            /**< If this is a system file, the corresponding configuration
                 key; otherwise, the null string.  System files may be disabled
                 but not deleted, and might use descriptive display names. */
        bool active_;
            /**< Whether or not this filename is currently active. */

    public:
        /**
         * Constructs an active non-system file.
         * The filename must be set using setFilename().
         */
        ReginaFilePref();

        /**
         * Constructs a non-system file with the given filename.
         * The display name will be set automatically.
         */
        ReginaFilePref(const QString& filename, bool active = true);

        /**
         * Constructs a system file with the given details.
         */
        ReginaFilePref(const QString& filename, const QString& displayName,
            const QString& systemKey, bool active = true);

        /**
         * Constructs a system file whose active status is read from the
         * given system settings.
         */
        ReginaFilePref(const QString& filename, const QString& displayName,
            const QString& systemKey, QSettings& settings);

        /**
         * Sets the filename, and sets the display name to a suitable
         * human-readable representation of the filename.
         */
        void setFilename(const QString& filename);

        /**
         * Marks this file as active.
         */
        void activate();

        /**
         * Marks this file as inactive.
         */
        void deactivate();

        /**
         * Returns a long human-readable display name.
         */
        const QString& longDisplayName() const;

        /**
         * Returns a short human-readable display name.
         */
        QString shortDisplayName() const;

        /**
         * Return the filename in an 8-bit encoding suitable for passing to
         * low-level file I/O routines.
         */
        QByteArray encodeFilename() const;

        /**
         * Indicates whether this file is currently active.
         */
        bool isActive() const;

        /**
         * Tests whether this file exists.
         */
        bool exists() const;

        /**
         * Indicates whether this is a system file.
         */
        bool isSystem() const;

        /**
         * Indicates whether this and the given preference use identical
         * filenames.
         * Note that different representations of the same file will
         * treated as different filenames (e.g., "foo/x" != "foo/../foo/x").
         */
        bool operator == (const ReginaFilePref& f) const;

    private:
        /**
         * Adds settings that stores all files in the given list (both
         * system and non-system files).
         * It is assumed that the settings group has already been set.
         *
         * When reading keys, the non-system files can be read through
         * readUserKey(), whereas the system files must be read via a
         * hard-coded list (since users should not be able to change
         * the hard-code list of system files).
         */
        static void writeKeys(const QList<ReginaFilePref>& list,
            QSettings& settings);

        /**
         * Reads the setting that lists all non-system files, and pushes
         * them onto the end of the given list.
         * It is assumed that the settings group has already been set.
         */
        static void readUserKey(QList<ReginaFilePref>& list,
            QSettings& settings);

    friend class ReginaPrefSet;
};

/**
 * A structure holding all Regina preferences.
 *
 * There is only one global object of type ReginaPrefSet; this ensures
 * that preferences are consistent across different main windows.
 *
 * You can access this global object by calling ReginaPrefSet.global().
 *
 * Normally you should only read the data members in this class.  If you
 * change them and you need other interface elements to notice your
 * changes (for instance, if some elements may need to redraw or refresh
 * themselves), then you should call propagate() to ensure that these
 * elements notice your changes.
 */
class ReginaPrefSet : public QObject {
    Q_OBJECT

    private:
        static ReginaPrefSet instance_;

        QList<QUrl> fileRecent_;
            /**< The actions that correspond to recently-opened files. */
        int fileRecentMax;
            /**< The maximum number of recent files to remember.
                 For now this is treated as read-only at runtime, though its
                 initial value is read from the configuration file. */

    public:
        // Some defaults that other classes may need to access:
        static const char* defaultGAPExec;
            /**< The default setting for \a triGAPExec. */

        // The preferences themselves:

        enum SurfacesCompatMatrix { LocalCompat, GlobalCompat };
            /**< Possible compatibility matrices that can be displayed for a
                 normal surface list. */

        bool anglesCreationTaut;
            /**< When enumerating angle structures, should the taut
                 structures option be enabled by default? */
        bool displayTagsInTree;
            /**< Should we display packet tags in the visual tree? */
        QByteArray fileImportExportCodec;
            /**< The codec to use for imports and exports. */
        bool helpIntroOnStartup;
            /**< Should we display introductory help on startup? */
        regina::HyperCoords hypersurfacesCreationCoords;
            /**< The default coordinate system for normal hypersurface
                 creation in a 4-manifold triangulation. */
        regina::HyperList hypersurfacesCreationList;
            /**< The default options for which normal hypersurfaces to
                 enumerate in a 4-manifold triangulation. */
        QString pdfExternalViewer;
            /**< The external program used to view PDFs.
                 This string may contain
                 additional arguments for the viewer; the PDF filename will
                 added to the end (separated by whitespace) and the entire
                 string will be passed to a shell for interpretation.  If this
                 is empty, Regina will do its best to find a suitable viewer. */
        bool pythonAutoIndent;
            /**< Should auto-indent be enabled in python consoles? */
        QList<ReginaFilePref> pythonLibraries;
            /**< The python libraries to load upon each session startup. */
        unsigned pythonSpacesPerTab;
            /**< The number of spaces to insert when <TAB> is pressed in a
                 python console. */
        bool pythonWordWrap;
            /**< Should python consoles be word wrapped? */
        unsigned surfacesCompatThreshold;
            /**< The maximum number of normal surfaces in a list for which the
                 compatibility matrices will be automatically calculated. */
        regina::NormalCoords surfacesCreationCoords;
            /**< The default coordinate system for normal surface creation
                 in a 3-manifold triangulation. */
        regina::NormalList surfacesCreationList;
            /**< The default options for which normal surfaces to enumerate
                 in a 3-manifold triangulation. */
        SurfacesCompatMatrix surfacesInitialCompat;
            /**< The matrix first shown when the compatibility tab is opened
                 for a normal surface list in a 3-manifold triangulation. */
        bool surfacesSupportOriented;
            /**< Should we offer support for transversely oriented normal
                 surfaces in 3-manifold triangulations?  This is still
                 highly experimental. */
        unsigned tabDim2Tri;
            /**< The index of the initial tab to open in a 2-manifold
                 triangulation viewer. */
        unsigned tabDim2TriSkeleton;
            /**< The index of the initial sub-tab to open in a 2-manifold
                 triangulation skeleton viewer. */
        unsigned tabDim3Tri;
            /**< The index of the initial tab to open in a 3-manifold
                 triangulation viewer. */
        unsigned tabDim3TriAlgebra;
            /**< The index of the initial sub-tab to open in a 3-manifold
                 triangulation algebra viewer. */
        unsigned tabDim3TriSkeleton;
            /**< The index of the initial sub-tab to open in a 3-manifold
                 triangulation skeleton viewer. */
        unsigned tabDim4Tri;
            /**< The index of the initial tab to open in a 4-manifold
                 triangulation viewer. */
        unsigned tabDim4TriAlgebra;
            /**< The index of the initial sub-tab to open in a 4-manifold
                 triangulation algebra viewer. */
        unsigned tabDim4TriSkeleton;
            /**< The index of the initial sub-tab to open in a 4-manifold
                 triangulation skeleton viewer. */
        unsigned tabHypersurfaceList;
            /**< The index of the initial sub-tab to open in a normal
                 hypersurface list viewer. */
        unsigned tabSnapPeaTri;
            /**< The index of the initial tab to open in a SnapPea
                 triangulation viewer. */
        unsigned tabSnapPeaTriAlgebra;
            /**< The index of the initial sub-tab to open in a SnapPea
                 triangulation algebra viewer. */
        unsigned tabSurfaceList;
            /**< The index of the initial sub-tab to open in a normal
                 surface list viewer. */
        unsigned treeJumpSize;
            /**< The number of steps corresponding to a jump up or down in
                 the packet tree. */
        QString triGAPExec;
            /**< The executable for starting GAP.  This need not include a
                 directory (in which case the search path will be used). */
        bool triGraphvizLabels;
            /**< Indicates whether tetrahedron/pentachoron numbers should
                 be used to label vertices in face/facet pairing graphs
                 for 3/4-manifold triangulations. */
        unsigned triSurfacePropsThreshold;
            /**< The maximum number of tetrahedra for which surface-related
                 properties of 3-manifold triangulations will be automatically
                 calculated. */

        bool warnOnNonEmbedded;
            /**< Whether to warn when attempting to enumerate normal surfaces
                 that are not embedded */

        QSize windowMainSize;
            /**< The initial size of a new main topology data window.
                 Note: as of Regina 5.0, the key for this option in the
                 configuration file has changed from \c MainSizeV2 to
                 \c MainSizeV3 (since another packet type was added, and
                 so again the window needs to be a little wider). */
        QSize windowPythonSize;
            /**< The initial size of a new python console. */

    public:
        /**
         * Access the global ReginaPrefSet instance.
         */
        static ReginaPrefSet& global();

        /**
         * Read the current preference list from the platform-specific
         * settings file.
         */
        static void read();

        /**
         * Save the current preference list to the platform-specific
         * settings file.
         */
        static void save();

        /**
         * Push a new URL to the front of the recent files list,
         * updating and reorganising the list as required.
         */
        static void addRecentFile(const QUrl& url);

        /**
         * Returns a read-only reference to the current list of recent URLs.
         */
        static const QList<QUrl>& recentFiles();

        /**
         * Returns the full path to the python libraries configuration file.
         * Under Windows (where python libraries are stored in the registry),
         * this routine returns the null string.
         */
        static QString pythonLibrariesConfig();

        /**
         * Returns a sensible fixed-width font.  The font size is not
         * specified.
         */
        static QFont fixedWidthFont();

        /**
         * Returns the preferred codec for imports and exports.
         * If the preferred codec could not be found, the UTF-8 codec
         * will be returned instead.
         *
         * This routine will never return null.
         */
        static QTextCodec* importExportCodec();

        /**
         * Opens the given section of an arbitrary handbook in an appropriate
         * manner.  If the handbook is in fact the users' handbook then
         * the argument \a handbook should be 0 (which enables specialised
         * code for the users' handbook only).
         */
        static void openHandbook(const char* section, const char* handbook,
            QWidget* parentWidget);

        /**
         * Inform everyone who needs to know that this preference set
         * might have changed.
         *
         * This simply causes the global ReginaPrefSet to emit the
         * signal preferencesChanged().
         */
        static void propagate();

    public slots:
        /**
         * Empty the recent files list.
         */
        void clearRecentFiles();

    signals:
        /**
         * Emitted from the global ReginaPrefSet instance when the
         * global preferences have changed.  If the recent files list
         * changes, this signal will \e not be emitted; however, one or
         * more of the recent files signals will be emitted instead.
         */
        void preferencesChanged();

        /**
         * Emitted from the global ReginaPrefSet instance when a new URL
         * is prepended to the beginning of the recent files list.
         */
        void recentFileAdded(const QUrl& url);

        /**
         * Emitted from the global ReginaPrefSet instance when an existing URL
         * is moved to the front of the recent files list.
         */
        void recentFilePromoted(const QUrl& url);

        /**
         * Emitted from the global ReginaPrefSet instance when the last URL
         * is removed from the recent files list.
         */
        void recentFileRemovedLast();

        /**
         * Emitted from the global ReginaPrefSet instance when the
         * recent files list is emptied completely.
         */
        void recentFilesCleared();

        /**
         * Emitted from the global ReginaPrefSet instance when the
         * recent files list is refilled from scratch, possibly bearing
         * no resemblance to its original contents.
         */
        void recentFilesFilled();

    private:
        // Constructor that provides a reasonable set of defaults.
        ReginaPrefSet();

        // Non-static internal read/write routines:
        void readInternal();
        void saveInternal() const;

        // Read and write python libraries to/from the regina-python
        // configuration file.  As an exception, under Windows, use the
        // registry instead.
        void readPythonLibraries();
        void savePythonLibraries() const;
};

inline ReginaFilePref::ReginaFilePref() : active_(true) {
}

inline ReginaFilePref::ReginaFilePref(const QString& filename, bool active) :
        active_(active) {
    setFilename(filename);
}

inline ReginaFilePref::ReginaFilePref(const QString& filename,
        const QString& displayName, const QString& systemKey, bool active) :
        filename_(filename), displayName_(displayName),
        systemKey_(systemKey), active_(active) {
}

inline bool ReginaFilePref::isActive() const {
    return active_;
}

inline bool ReginaFilePref::isSystem() const {
    return (! systemKey_.isNull());
}

inline void ReginaFilePref::activate() {
    active_ = true;
}

inline void ReginaFilePref::deactivate() {
    active_ = false;
}

inline const QString& ReginaFilePref::longDisplayName() const {
    return displayName_;
}

inline bool ReginaFilePref::operator == (const ReginaFilePref& f) const {
    return (filename_ == f.filename_);
}

inline ReginaPrefSet& ReginaPrefSet::global() {
    return instance_;
}

inline void ReginaPrefSet::read() {
    instance_.readInternal();
}

inline void ReginaPrefSet::save() {
    instance_.saveInternal();
}

inline void ReginaPrefSet::propagate() {
    emit instance_.preferencesChanged();
}

inline const QList<QUrl>& ReginaPrefSet::recentFiles() {
    return instance_.fileRecent_;
}

#endif

