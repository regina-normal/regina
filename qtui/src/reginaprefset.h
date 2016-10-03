
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
        enum TriGraphType { DualGraph, TreeDecomposition,
                NiceTreeDecomposition };
            /**< Possible types of graph that can be displayed in a
                 (2,3,4)-manifold graph viewer. */

        bool anglesCreationTaut;
            /**< When enumerating angle structures, should the taut
                 structures option be enabled by default? */
        bool displayTagsInTree;
            /**< Should we display packet tags in the visual tree? */
        bool displayUnicode;
            /**< Should we use unicode liberally throughout the GUI? */
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
        TriGraphType triInitialGraphType;
            /**< Indicates which graph to initially display in a
                 (2,3,4)-manifold graph viewer. */
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
};

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

