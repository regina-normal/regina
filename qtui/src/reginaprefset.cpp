
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

#include "regina-config.h"
#include "file/nglobaldirs.h"
#include "snappea/nsnappeatriangulation.h"
#include "surfaces/nnormalsurfacelist.h"
#include "utilities/stringutils.h"

#include "reginaprefset.h"
#include "reginasupport.h"
#include "shortrunner.h"

#include <fstream>
#include <qglobal.h>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>

namespace {
    QString INACTIVE("## INACTIVE ##");
}

ReginaPrefSet ReginaPrefSet::instance_;

#ifdef Q_OS_WIN32
    const char* ReginaPrefSet::defaultGAPExec = "gap.exe";
#else
    const char* ReginaPrefSet::defaultGAPExec = "gap";
#endif

ReginaFilePref::ReginaFilePref(const QString& filename,
        const QString& displayName, const QString& systemKey,
        QSettings& settings) :
        filename_(filename), displayName_(displayName), systemKey_(systemKey) {
    active_ = settings.value(systemKey_, true).toBool();
}

void ReginaFilePref::setFilename(const QString& filename) {
    filename_ = filename;
    displayName_ = QFileInfo(filename_).fileName() + " [" + filename_ + ']';
}

bool ReginaFilePref::exists() const {
    return QFileInfo(filename_).exists();
}

QString ReginaFilePref::shortDisplayName() const {
    if (systemKey_.isNull())
        return QFileInfo(filename_).fileName();
    else
        return displayName_;
}

QByteArray ReginaFilePref::encodeFilename() const {
    return static_cast<const char*>(QFile::encodeName(filename_));
}

void ReginaFilePref::writeKeys(const QList<ReginaFilePref>& list,
        QSettings& settings) {
    QStringList strings;
    foreach (const ReginaFilePref& f, list) {
        if (f.systemKey_.isNull())
            strings.push_back((f.active_ ? '+' : '-') + f.filename_);
        else
            settings.setValue(f.systemKey_, f.active_);
    }
    settings.setValue("UserFiles", strings);
}

void ReginaFilePref::readUserKey(QList<ReginaFilePref>& list,
        QSettings& settings) {
    QStringList strings = 
        settings.value("UserFiles", QStringList()).value<QStringList>();

    // Each string must start with + or - (active or inactive).
    // Any other strings will be ignored.
    for (QStringList::const_iterator it = strings.begin();
            it != strings.end(); it++) {
        if ((*it).isEmpty())
            continue;
        // Use toLatin1() since we're just testing a char for '+' or '-'.
        switch ((*it)[0].toLatin1()) {
            case '+':
                // Active file.
                list.push_back(ReginaFilePref((*it).mid(1), true));
                break;
            case '-':
                // Inactive file.
                list.push_back(ReginaFilePref((*it).mid(1), false));
                break;
        }
    }
}

ReginaPrefSet::ReginaPrefSet() :
        anglesCreationTaut(false),
        fileRecentMax(10),
        displayTagsInTree(false),
        displayUnicode(true),
        fileImportExportCodec("UTF-8"),
        helpIntroOnStartup(true),
        hypersurfacesCreationCoords(regina::HS_STANDARD),
        hypersurfacesCreationList(regina::HS_LIST_DEFAULT),
        pythonAutoIndent(true),
        pythonSpacesPerTab(4),
        pythonWordWrap(false),
        surfacesCompatThreshold(100),
        surfacesCreationCoords(regina::NS_STANDARD),
        surfacesCreationList(regina::NS_LIST_DEFAULT),
        surfacesInitialCompat(LocalCompat),
        surfacesSupportOriented(false),
        treeJumpSize(10),
        tabDim2Tri(0),
        tabDim2TriSkeleton(0),
        tabDim3Tri(0),
        tabDim3TriAlgebra(0),
        tabDim3TriSkeleton(0),
        tabDim4Tri(0),
        tabDim4TriAlgebra(0),
        tabDim4TriSkeleton(0),
        tabHypersurfaceList(0),
        tabSnapPeaTri(0),
        tabSnapPeaTriAlgebra(0),
        tabSurfaceList(0),
        triGAPExec(defaultGAPExec),
        triGraphvizLabels(true),
        triSurfacePropsThreshold(6),
        warnOnNonEmbedded(true) {
}

QString ReginaPrefSet::pythonLibrariesConfig() {
#ifdef Q_OS_WIN32
    return QString(); // Use the registry instead.
#else
    return QDir::homePath() + "/.regina-libs";
#endif
}

void ReginaPrefSet::readPythonLibraries() {
    QString configFile = pythonLibrariesConfig();
    if (configFile.isNull()) {
        QSettings pySettings(QCoreApplication::organizationDomain(),
            "Regina-Python");

        pySettings.beginGroup("PythonLibraries");
        pythonLibraries.clear();
        ReginaFilePref::readUserKey(pythonLibraries, pySettings);
        pySettings.endGroup();
    } else {
        pythonLibraries.clear();

        QFile f(configFile);
        if (! f.open(QIODevice::ReadOnly))
            return /* false */;

        QTextStream in(&f);
        in.setCodec(QTextCodec::codecForName("UTF-8"));

        bool active;
        QString line = in.readLine();
        while (! line.isNull()) {
            // Is the file inactive?
            active = true;
            if (line.startsWith(INACTIVE)) {
                active = false;
                line = line.mid(INACTIVE.length());
            }

            line = line.trimmed();

            // Is it a file at all?  If so, add it.
            if ((! line.isEmpty()) && line[0] != '#')
                pythonLibraries.push_back(ReginaFilePref(line, active));

            // Next!
            line = in.readLine();
        }

        return /* true */;
    }
}

void ReginaPrefSet::savePythonLibraries() const {
    QString configFile = pythonLibrariesConfig();
    if (configFile.isNull()) {
        QSettings pySettings(QCoreApplication::organizationDomain(),
            "Regina-Python");

        pySettings.beginGroup("PythonLibraries");
        ReginaFilePref::writeKeys(pythonLibraries, pySettings);
        pySettings.endGroup();
    } else {
        QFile f(configFile);
        if (! f.open(QIODevice::WriteOnly))
            return /* false */;

        QTextStream out(&f);
        out.setCodec(QTextCodec::codecForName("UTF-8"));

        out << "# Python libraries configuration file\n#\n";
        out << "# Automatically generated by the graphical user interface.\n\n";

        foreach (const ReginaFilePref& f, pythonLibraries) {
            if (f.active_)
                out << f.filename_ << '\n';
            else
                out << INACTIVE << ' ' << f.filename_ << '\n';
        }

        return /* true */;
    }
}

QFont ReginaPrefSet::fixedWidthFont() {
    QFont ans;
#ifdef Q_OS_MACX
    ans.setFamily("menlo");
#else
    ans.setFamily("monospace");
#endif
    ans.setFixedPitch(true);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
    ans.setStyleHint(QFont::Monospace);
#else
    ans.setStyleHint(QFont::TypeWriter);
#endif
    return ans;
}

void ReginaPrefSet::openHandbook(const char* section, const char* handbook,
        QWidget* parentWidget) {
    QString handbookName = (handbook ? handbook : "regina");

    QString home = QFile::decodeName(regina::NGlobalDirs::home().c_str());
    QString index = home +
        QString("/docs/en/%1/index.html").arg(handbookName);
    QString page = home +
        QString("/docs/en/%1/%2.html").arg(handbookName).arg(section);
    if (QFileInfo(page).exists()) {
        if (! QDesktopServices::openUrl(QUrl::fromLocalFile(page))) {
            if (handbook) {
                ReginaSupport::warn(parentWidget,
                    QObject::tr("I could not open the requested handbook."),
                    QObject::tr("<qt>Please try pointing your web browser to: "
                    "<tt>%1</tt></qt>").arg(page.toHtmlEscaped()));
            } else {
                ReginaSupport::warn(parentWidget,
                    QObject::tr("I could not open the Regina handbook."),
                    QObject::tr("<qt>Please try pointing your web browser to: "
                    "<tt>%1</tt></qt>").arg(page.toHtmlEscaped()));
            }
        }
    } else {
        if (handbook) {
            ReginaSupport::warn(parentWidget,
                QObject::tr("I could not find the requested handbook."),
                QObject::tr("<qt>It should be installed at: "
                "<tt>%1</tt><p>Please contact %2 for assistance.</qt>")
                .arg(index.toHtmlEscaped()).arg(PACKAGE_BUGREPORT));
        } else {
            ReginaSupport::warn(parentWidget,
                QObject::tr("I could not find the Regina handbook."),
                QObject::tr("<qt>It should be installed at: "
                "<tt>%1</tt><p>Please contact %2 for assistance.</qt>")
                .arg(index.toHtmlEscaped()).arg(PACKAGE_BUGREPORT));
        }
    }
}

/*
void ReginaPrefSet::setMaxItems(int newMax) {
    fileRecentMax_ = newMax;
    if (newMax <= 0)
        return;

    while (fileRecent_.size() > newMax) {
        QUrl u = fileRecent_.back();
        fileRecent_.pop_back();
        emit recentFileRemoved(u);
    }
}
*/

void ReginaPrefSet::addRecentFile(const QUrl& url) {
    for (int i = 0; i < instance_.fileRecent_.count(); ++i)
        if (instance_.fileRecent_[i] == url) {
            instance_.fileRecent_.move(i, 0);
            emit instance_.recentFilePromoted(url);
            return;
        }

    if (instance_.fileRecentMax > 0 &&
            instance_.fileRecent_.size() >= instance_.fileRecentMax) {
        instance_.fileRecent_.pop_back();
        emit instance_.recentFileRemovedLast();
    }

    instance_.fileRecent_.push_front(url);
    emit instance_.recentFileAdded(url);
}

void ReginaPrefSet::clearRecentFiles() {
    fileRecent_.clear();
    emit recentFilesCleared();
}

void ReginaPrefSet::readInternal() {
    QSettings settings;

    settings.beginGroup("Angles");
    anglesCreationTaut = settings.value("CreationTaut", false).toBool();
    settings.endGroup();

    settings.beginGroup("Display");
    displayTagsInTree = settings.value("DisplayTagsInTree", false).toBool();
    displayUnicode = settings.value("DisplayUnicode", true).toBool();
    settings.endGroup();

    settings.beginGroup("File");
    fileRecentMax = settings.value("RecentMax", 10).toInt();
    fileImportExportCodec = settings.value("ImportExportCodec",
        QByteArray("UTF-8")).toByteArray();
    settings.endGroup();

    settings.beginGroup("Help");
    helpIntroOnStartup = settings.value("IntroOnStartup", true).toBool();
    settings.endGroup();

    settings.beginGroup("Hypersurfaces");
    hypersurfacesCreationCoords = static_cast<regina::HyperCoords>(
        settings.value("CreationCoordinates", regina::HS_STANDARD).toInt());
    hypersurfacesCreationList = regina::HyperList::fromInt(settings.value(
        "CreationList", regina::HS_LIST_DEFAULT).toInt());

    settings.beginGroup("Python");
    pythonAutoIndent = settings.value("AutoIndent", true).toBool();
    pythonSpacesPerTab = settings.value("SpacesPerTab", 4).toInt();
    pythonWordWrap = settings.value("WordWrap", false).toBool();
    settings.endGroup();

    settings.beginGroup("SnapPea");
    regina::NSnapPeaTriangulation::enableKernelMessages(
        settings.value("KernelMessages", false).toBool());
    settings.endGroup();

    settings.beginGroup("Surfaces");
    surfacesCompatThreshold = settings.value(
        "CompatibilityThreshold", 100).toInt();
    surfacesCreationCoords = static_cast<regina::NormalCoords>(settings.value(
        "CreationCoordinates", regina::NS_STANDARD).toInt());
    surfacesCreationList = regina::NormalList::fromInt(settings.value(
        "CreationList", regina::NS_LIST_DEFAULT).toInt());

    QString str = settings.value("InitialCompat").toString();
    if (str == "Global")
        surfacesInitialCompat = ReginaPrefSet::GlobalCompat;
    else
        surfacesInitialCompat = ReginaPrefSet::LocalCompat; /* default */

    surfacesSupportOriented = settings.value("SupportOriented", false).toBool();
    warnOnNonEmbedded = settings.value("WarnOnNonEmbedded", true).toBool();
    settings.endGroup();

    settings.beginGroup("Tree");
    treeJumpSize = settings.value("JumpSize", 10).toInt();
    settings.endGroup();

    settings.beginGroup("Tabs");
    tabDim2Tri = settings.value("Dim2Tri", 0).toUInt();
    tabDim2TriSkeleton = settings.value("Dim2TriSkeleton", 0).toUInt();
    tabDim3Tri = settings.value("Dim3Tri", 0).toUInt();
    tabDim3TriAlgebra = settings.value("Dim3TriAlgebra", 0).toUInt();
    tabDim3TriSkeleton = settings.value("Dim3TriSkeleton", 0).toUInt();
    tabDim4Tri = settings.value("Dim4Tri", 0).toUInt();
    tabDim4TriAlgebra = settings.value("Dim4TriAlgebra", 0).toUInt();
    tabDim4TriSkeleton = settings.value("Dim4TriSkeleton", 0).toUInt();
    tabHypersurfaceList = settings.value("HypersurfaceList", 0).toUInt();
    tabSnapPeaTri = settings.value("SnapPeaTri", 0).toUInt();
    tabSnapPeaTriAlgebra = settings.value("SnapPeaTriAlgebra", 0).toUInt();
    tabSurfaceList = settings.value("SurfaceList", 0).toUInt();
    settings.endGroup();

    settings.beginGroup("Triangulation");
    triGraphvizLabels = settings.value("GraphvizLabels", true).toBool();
    triSurfacePropsThreshold = settings.value(
        "SurfacePropsThreshold", 6).toInt();
    settings.endGroup();

    settings.beginGroup("Tools");
    pdfExternalViewer = settings.value("PDFViewer").toString().trimmed();
    triGAPExec = settings.value("GAPExec", defaultGAPExec).toString().trimmed();
    settings.endGroup();

    readPythonLibraries();

    settings.beginGroup("Window");
    windowMainSize = settings.value("MainSizeV3", QSize()).toSize();
    windowPythonSize = settings.value("PythonSize", QSize()).toSize();
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    fileRecent_.clear();
    QString val;
    for (int i = 0; i < fileRecentMax; ++i) {
        val = settings.value(QString("File%1").arg(i + 1)).toString();
        if (val.isEmpty())
            continue;
        if (! QFile::exists(val))
            continue;
        fileRecent_.push_back(QUrl::fromLocalFile(val));
    }
    settings.endGroup();

    emit instance_.preferencesChanged();
    emit instance_.recentFilesFilled();
}

void ReginaPrefSet::saveInternal() const {
    QSettings settings;
    settings.beginGroup("Angles");
    settings.setValue("CreationTaut", anglesCreationTaut);
    settings.endGroup();

    settings.beginGroup("Display");
    settings.setValue("DisplayTagsInTree", displayTagsInTree);
    settings.setValue("DisplayUnicode", displayUnicode);
    settings.endGroup();

    settings.beginGroup("File");
    settings.setValue("RecentMax", fileRecentMax);
    settings.setValue("ImportExportCodec", fileImportExportCodec);
    settings.endGroup();

    settings.beginGroup("Help");
    settings.setValue("IntroOnStartup", helpIntroOnStartup);
    settings.endGroup();

    settings.beginGroup("Hypersurfaces");
    settings.setValue("CreationCoordinates", hypersurfacesCreationCoords);
    settings.setValue("CreationList", hypersurfacesCreationList.intValue());

    settings.beginGroup("Python");
    settings.setValue("AutoIndent", pythonAutoIndent);
    settings.setValue("SpacesPerTab", pythonSpacesPerTab);
    settings.setValue("WordWrap", pythonWordWrap);
    settings.endGroup();

    settings.beginGroup("SnapPea");
    settings.setValue("KernelMessages",
        regina::NSnapPeaTriangulation::kernelMessagesEnabled());
    settings.endGroup();

    settings.beginGroup("Surfaces");
    settings.setValue("CompatibilityThreshold", surfacesCompatThreshold);
    settings.setValue("CreationCoordinates", surfacesCreationCoords);
    settings.setValue("CreationList", surfacesCreationList.intValue());

    switch (surfacesInitialCompat) {
        case ReginaPrefSet::GlobalCompat:
            settings.setValue("InitialCompat", "Global"); break;
        default:
            settings.setValue("InitialCompat", "Local"); break;
    }

    settings.setValue("SupportOriented", surfacesSupportOriented);
    settings.setValue("WarnOnNonEmbedded", warnOnNonEmbedded);
    settings.endGroup();

    settings.beginGroup("Tabs");
    settings.setValue("Dim2Tri", tabDim2Tri);
    settings.setValue("Dim2TriSkeleton", tabDim2TriSkeleton);
    settings.setValue("Dim3Tri", tabDim3Tri);
    settings.setValue("Dim3TriAlgebra", tabDim3TriAlgebra);
    settings.setValue("Dim3TriSkeleton", tabDim3TriSkeleton);
    settings.setValue("Dim4Tri", tabDim4Tri);
    settings.setValue("Dim4TriAlgebra", tabDim4TriAlgebra);
    settings.setValue("Dim4TriSkeleton", tabDim4TriSkeleton);
    settings.setValue("HypersurfaceList", tabHypersurfaceList);
    settings.setValue("SnapPeaTri", tabSnapPeaTri);
    settings.setValue("SnapPeaTriAlgebra", tabSnapPeaTriAlgebra);
    settings.setValue("SurfaceList", tabSurfaceList);
    settings.endGroup();

    settings.beginGroup("Tree");
    settings.setValue("JumpSize", treeJumpSize);
    settings.endGroup();

    settings.beginGroup("Triangulation");
    settings.setValue("GraphvizLabels", triGraphvizLabels);
    settings.setValue("SurfacePropsThreshold", triSurfacePropsThreshold);
    settings.endGroup();

    settings.beginGroup("Tools");
    settings.setValue("PDFViewer", pdfExternalViewer);
    settings.setValue("GAPExec", triGAPExec);
    settings.endGroup();

    savePythonLibraries();

    settings.beginGroup("Window");
    settings.setValue("MainSizeV3", windowMainSize);
    settings.setValue("PythonSize", windowPythonSize);
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    settings.remove("");
    for (int i = 0; i < fileRecent_.count(); ++i)
        settings.setValue(QString("File%1").arg(i + 1),
            fileRecent_[i].toLocalFile());
    settings.endGroup();
}

QTextCodec* ReginaPrefSet::importExportCodec() {
    QTextCodec* ans = QTextCodec::codecForName(global().fileImportExportCodec);
    if (! ans)
        ans = QTextCodec::codecForName("UTF-8");
    return ans;
}

