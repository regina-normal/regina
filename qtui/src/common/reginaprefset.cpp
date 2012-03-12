
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include "regina-config.h"
#include "file/nglobaldirs.h"
#include "snappea/nsnappeatriangulation.h"
#include "surfaces/nnormalsurfacelist.h"
#include "utilities/stringutils.h"

#include "reginaprefset.h"
#include "reginasupport.h"
#include "shortrunner.h"

#include <fstream>
#include <QApplication>
#include <QDesktopWidget>
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

const GraphvizStatus GraphvizStatus::unknown(0);
const GraphvizStatus GraphvizStatus::notFound(-1);
const GraphvizStatus GraphvizStatus::notExist(-2);
const GraphvizStatus GraphvizStatus::notExecutable(-3);
const GraphvizStatus GraphvizStatus::notStartable(-4);
const GraphvizStatus GraphvizStatus::unsupported(-5);
const GraphvizStatus GraphvizStatus::version1(1);
const GraphvizStatus GraphvizStatus::version1NotDot(2);
const GraphvizStatus GraphvizStatus::version2(3);

ReginaPrefSet ReginaPrefSet::instance_;

const char* ReginaPrefSet::defaultGAPExec = "gap";
const char* ReginaPrefSet::defaultGraphvizExec = "neato";

// No need to initialise these, since the cache is only used when the
// given executable matches cacheGraphvizExec (which begins life as null).
QMutex GraphvizStatus::cacheGraphvizMutex;
QString GraphvizStatus::cacheGraphvizExec;
QString GraphvizStatus::cacheGraphvizExecFull;
GraphvizStatus GraphvizStatus::cacheGraphvizStatus;

QByteArray ReginaFilePref::encodeFilename() const {
    return static_cast<const char*>(QFile::encodeName(filename));
}

GraphvizStatus GraphvizStatus::status(const QString& userExec,
        QString& fullExec, bool forceRecheck) {
    QMutexLocker lock(&cacheGraphvizMutex);

    if ((! forceRecheck) && cacheGraphvizStatus != unknown &&
            userExec == cacheGraphvizExec) {
        fullExec = cacheGraphvizExecFull;
        return cacheGraphvizStatus;
    }

    // We need a full requery.
    if (! userExec.contains(QDir::separator())) {
        // Hunt on the search path.
        QString paths = QProcessEnvironment::systemEnvironment().value("PATH");
        // Windows uses a different separator in $PATH
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN
        QString pathSeparator = ";";
#else
        QString pathSeparator = ":";
#endif
        QStringList pathList = paths.split(pathSeparator);
        bool found = false;
        for( QStringList::iterator it = pathList.begin(); it != pathList.end();
            ++it) {
            QDir dir(*it);
            if ( dir.exists(userExec) ) {
                fullExec = dir.absoluteFilePath(userExec);
                found = true;
                break;
            }
        }
        if (! found) {
            fullExec = QString();
            return notFound;
        }
    } else
        fullExec = QFileInfo(userExec).absoluteFilePath();

    // We have a full path to the Graphviz executable.
    QFileInfo info(fullExec);
    if (! info.exists())
        return notExist;
    if (! (info.isFile() && info.isExecutable()))
        return notExecutable;

    // Run to extract a version string.
    ShortRunner graphviz;
    graphviz << fullExec << "-V";
    QString output = graphviz.run(true);
    if (output.isNull()) {
        if (graphviz.timedOut())
            return unsupported;
        else
            return notStartable;
    }

    if (output.contains("version 1.")) {
        // Only test for "dot", not "/dot".  I'd rather not get tripped
        // up with alternate path separators, and this still
        // distinguishes between the different 1.x graph drawing tools.
        if (userExec.endsWith("dot", Qt::CaseInsensitive))
            return version1;
        return version1NotDot;
    } else if (output.contains("version 0."))
        return unsupported;
    else if (output.contains("version")) {
        // Assume any other version is >= 2.x.
        return version2;
    } else {
        // Could not find a version string at all.
        return unsupported;
    }
}

ReginaPrefSet::ReginaPrefSet() :
        autoDock(true),
        censusFiles(defaultCensusFiles()),
        displayTagsInTree(false),
        fileRecentMax(10),
        pdfAutoClose(true),
        pythonAutoIndent(true),
        pythonSpacesPerTab(4),
        pythonWordWrap(false),
        snapPeaClosed(false),
        surfacesCompatThreshold(100),
        surfacesCreationCoords(regina::NNormalSurfaceList::STANDARD),
        surfacesInitialCompat(LocalCompat),
        surfacesInitialTab(Summary),
        treeJumpSize(10),
        triGAPExec(defaultGAPExec),
        triGraphvizExec(defaultGraphvizExec),
        triInitialTab(Gluings),
        triInitialSkeletonTab(SkelComp),
        triInitialAlgebraTab(Homology),
        triSurfacePropsThreshold(6),
        warnOnNonEmbedded(true),
        windowMainSize() /* initially null */ {
}

QSize ReginaPrefSet::defaultMainSize() {
    // Use roughly 2/3 the screen size by default.
    QSize use = QApplication::desktop()->availableGeometry().size();
    use *= 2;
    use /= 3;
    return use;
}

ReginaFilePrefList ReginaPrefSet::defaultCensusFiles() {
    ReginaFilePrefList ans;

    QString exDir = QFile::decodeName(regina::NGlobalDirs::examples().c_str());
    ans.push_back(ReginaFilePref(exDir + "/closed-or-census.rga"));
    ans.push_back(ReginaFilePref(exDir + "/closed-nor-census.rga"));
    ans.push_back(ReginaFilePref(exDir + "/knot-link-census.rga"));
    ans.push_back(ReginaFilePref(exDir + "/snappea-census.rga"));
    ans.push_back(ReginaFilePref(exDir + "/closed-hyp-census.rga"));

    return ans;
}

QString ReginaPrefSet::pythonLibrariesConfig() {
    return QDir::homePath() + "/.regina-libs";
}

bool ReginaPrefSet::readPythonLibraries() {
    pythonLibraries.clear();

    QFile f(pythonLibrariesConfig());
    if (! f.open(QIODevice::ReadOnly))
        return false;

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

    return true;
}

bool ReginaPrefSet::savePythonLibraries() const {
    QFile f(pythonLibrariesConfig());
    if (! f.open(QIODevice::WriteOnly))
        return false;

    QTextStream out(&f);
    out.setCodec(QTextCodec::codecForName("UTF-8"));

    out << "# Python libraries configuration file\n#\n";
    out << "# Automatically generated by the KDE user interface.\n\n";

    for (ReginaFilePrefList::const_iterator it = pythonLibraries.begin();
            it != pythonLibraries.end(); it++)
        if ((*it).active)
            out << (*it).filename << '\n';
        else
            out << INACTIVE << ' ' << (*it).filename << '\n';

    return true;
}

QFont ReginaPrefSet::fixedWidthFont() {
    QFont ans;
#ifdef Q_OS_MACX
    ans.setFamily("menlo");
#else
    ans.setFamily("monospace");
#endif
    ans.setFixedPitch(true);
    ans.setStyleHint(QFont::Monospace);
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
                    "<tt>%1</tt></qt>").arg(Qt::escape(page)));
            } else {
                ReginaSupport::warn(parentWidget,
                    QObject::tr("I could not open the Regina handbook."),
                    QObject::tr("<qt>Please try pointing your web browser to: "
                    "<tt>%1</tt></qt>").arg(Qt::escape(page)));
            }
        }
    } else {
        if (handbook) {
            ReginaSupport::warn(parentWidget,
                QObject::tr("I could not find the requested handbook."),
                QObject::tr("<qt>It should be installed at: "
                "<tt>%1</tt><p>Please contact %2 for assistance.</qt>")
                .arg(Qt::escape(index)).arg(PACKAGE_BUGREPORT));
        } else {
            ReginaSupport::warn(parentWidget,
                QObject::tr("I could not find the Regina handbook."),
                QObject::tr("<qt>It should be installed at: "
                "<tt>%1</tt><p>Please contact %2 for assistance.</qt>")
                .arg(Qt::escape(index)).arg(PACKAGE_BUGREPORT));
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

    settings.beginGroup("Display");
    autoDock = settings.value("PacketDocking", true).toBool();
    displayTagsInTree = settings.value("DisplayTagsInTree", false).toBool();
    settings.endGroup();

    settings.beginGroup("Census");
    QStringList censusStrings = 
        settings.value("Files", QStringList()).value<QStringList>();
    if (censusStrings.empty())
        censusFiles = ReginaPrefSet::defaultCensusFiles();
    else {
        censusFiles.clear();

        // Each string must start with + or - (active or inactive).
        // Any other strings will be ignored.
        for (QStringList::const_iterator it = censusStrings.begin();
                it != censusStrings.end(); it++) {
            if ((*it).isEmpty())
                continue;
            switch ((*it)[0].toAscii()) {
                case '+':
                    // Active file.
                    censusFiles.push_back(ReginaFilePref((*it).mid(1), true));
                    break;
                case '-':
                    // Inactive file.
                    censusFiles.push_back(ReginaFilePref((*it).mid(1), false));
                    break;
            }
        }
    }
    settings.endGroup();
    
    settings.beginGroup("File");
    fileRecentMax = settings.value("RecentMax", 10).toInt();
    settings.endGroup();

    settings.beginGroup("PDF");
    pdfAutoClose = settings.value("AutoClose", true).toBool();
    pdfExternalViewer = settings.value("ExternalViewer").toString().trimmed();
    settings.endGroup();

    settings.beginGroup("Python");
    pythonAutoIndent = settings.value("AutoIndent", true).toBool();
    pythonSpacesPerTab = settings.value("SpacesPerTab", 4).toInt();
    pythonWordWrap = settings.value("WordWrap", false).toBool();
    settings.endGroup();

    settings.beginGroup("SnapPea");
    snapPeaClosed = settings.value("AllowClosed", false).toBool();
    regina::NSnapPeaTriangulation::enableKernelMessages(
        settings.value("KernelMessages", false).toBool());
    settings.endGroup();

    settings.beginGroup("Surfaces");
    surfacesCompatThreshold = settings.value(
        "CompatibilityThreshold", 100).toInt();
    surfacesCreationCoords = settings.value(
        "CreationCoordinates", regina::NNormalSurfaceList::STANDARD).toInt();

    QString str = settings.value("InitialCompat").toString();
    if (str == "Global")
        surfacesInitialCompat = ReginaPrefSet::GlobalCompat;
    else
        surfacesInitialCompat = ReginaPrefSet::LocalCompat; /* default */

    str = settings.value("InitialTab").toString();
    if (str == "Coordinates")
        surfacesInitialTab = ReginaPrefSet::Coordinates;
    else if (str == "Matching")
        surfacesInitialTab = ReginaPrefSet::Matching;
    else if (str == "Compatibility")
        surfacesInitialTab = ReginaPrefSet::Compatibility;
    else
        surfacesInitialTab = ReginaPrefSet::Summary; /* default */
    settings.endGroup();

    settings.beginGroup("Tree");
    treeJumpSize = settings.value("JumpSize", 10).toInt();
    settings.endGroup();

    settings.beginGroup("Triangulation");

    str = settings.value("InitialTab").toString();
    if (str == "Skeleton")
        triInitialTab = ReginaPrefSet::Skeleton;
    else if (str == "Algebra")
        triInitialTab = ReginaPrefSet::Algebra;
    else if (str == "Composition")
        triInitialTab = ReginaPrefSet::Composition;
    else if (str == "Surfaces")
        triInitialTab = ReginaPrefSet::Surfaces;
    else if (str == "SnapPea")
        triInitialTab = ReginaPrefSet::SnapPea;
    else
        triInitialTab = ReginaPrefSet::Gluings; /* default */

    str = settings.value("InitialSkeletonTab").toString();
    if (str == "FacePairingGraph")
        triInitialSkeletonTab = ReginaPrefSet::FacePairingGraph;
    else
        triInitialSkeletonTab = ReginaPrefSet::SkelComp; /* default */

    str = settings.value("InitialAlgebraTab").toString();
    if (str == "FundGroup")
        triInitialAlgebraTab = ReginaPrefSet::FundGroup;
    else if (str == "TuraevViro")
        triInitialAlgebraTab = ReginaPrefSet::TuraevViro;
    else if (str == "CellularInfo")
        triInitialAlgebraTab = ReginaPrefSet::CellularInfo;
    else
        triInitialAlgebraTab = ReginaPrefSet::Homology; /* default */

    triSurfacePropsThreshold = settings.value(
        "SurfacePropsThreshold", 6).toInt();
    settings.endGroup();

    settings.beginGroup("Extensions");
    triGAPExec = settings.value("GAPExec", "gap").toString().trimmed();
    triGraphvizExec = settings.value("GraphvizExec", "neato").
        toString().trimmed();
    settings.endGroup();

    readPythonLibraries();

    settings.beginGroup("Window");
    windowMainSize = settings.value("MainSize", defaultMainSize()).toSize();
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
    settings.beginGroup("Display");
    // Save the current set of preferences.
    settings.setValue("PacketDocking", autoDock);
    settings.setValue("PacketDocking", autoDock);
    settings.setValue("DisplayTagsInTree", displayTagsInTree);
    settings.endGroup();

    settings.beginGroup("Census");
    QStringList censusStrings;
    // Distinguish an empty list from an uninitialised list.
    if (censusFiles.empty())
        censusStrings.push_back("0");
    else
        for (ReginaFilePrefList::const_iterator it = censusFiles.begin();
                it != censusFiles.end(); it++)
            censusStrings.push_back(((*it).active ? '+' : '-') +
                (*it).filename);
    settings.setValue("Files", censusStrings);
    settings.endGroup();

    settings.beginGroup("File");
    settings.setValue("RecentMax", fileRecentMax);
    settings.endGroup();

    settings.beginGroup("PDF");
    settings.setValue("AutoClose", pdfAutoClose);
    settings.setValue("ExternalViewer", pdfExternalViewer);
    settings.endGroup();

    settings.beginGroup("Python");
    settings.setValue("AutoIndent", pythonAutoIndent);
    settings.setValue("SpacesPerTab", pythonSpacesPerTab);
    settings.setValue("WordWrap", pythonWordWrap);
    settings.endGroup();

    settings.beginGroup("SnapPea");
    settings.setValue("AllowClosed", snapPeaClosed);
    settings.setValue("KernelMessages",
        regina::NSnapPeaTriangulation::kernelMessagesEnabled());
    settings.endGroup();

    settings.beginGroup("Surfaces");
    settings.setValue("CompatibilityThreshold", surfacesCompatThreshold);
    settings.setValue("CreationCoordinates", surfacesCreationCoords);

    switch (surfacesInitialCompat) {
        case ReginaPrefSet::GlobalCompat:
            settings.setValue("InitialCompat", "Global"); break;
        default:
            settings.setValue("InitialCompat", "Local"); break;
    }

    switch (surfacesInitialTab) {
        case ReginaPrefSet::Coordinates:
            settings.setValue("InitialTab", "Coordinates"); break;
        case ReginaPrefSet::Matching:
            settings.setValue("InitialTab", "Matching"); break;
        case ReginaPrefSet::Compatibility:
            settings.setValue("InitialTab", "Compatibility"); break;
        default:
            settings.setValue("InitialTab", "Summary"); break;
    }
    settings.endGroup();

    settings.beginGroup("Tree");
    settings.setValue("JumpSize", treeJumpSize);
    settings.endGroup();

    settings.beginGroup("Triangulation");

    switch (triInitialTab) {
        case ReginaPrefSet::Skeleton:
            settings.setValue("InitialTab", "Skeleton"); break;
        case ReginaPrefSet::Algebra:
            settings.setValue("InitialTab", "Algebra"); break;
        case ReginaPrefSet::Composition:
            settings.setValue("InitialTab", "Composition"); break;
        case ReginaPrefSet::Surfaces:
            settings.setValue("InitialTab", "Surfaces"); break;
        case ReginaPrefSet::SnapPea:
            settings.setValue("InitialTab", "SnapPea"); break;
        default:
            settings.setValue("InitialTab", "Gluings"); break;
    }

    switch (triInitialSkeletonTab) {
        case ReginaPrefSet::FacePairingGraph:
            settings.setValue("InitialSkeletonTab", "FacePairingGraph"); break;
        default:
            settings.setValue("InitialSkeletonTab", "SkelComp"); break;
    }

    switch (triInitialAlgebraTab) {
        case ReginaPrefSet::FundGroup:
            settings.setValue("InitialAlgebraTab", "FundGroup"); break;
        case ReginaPrefSet::TuraevViro:
            settings.setValue("InitialAlgebraTab", "TuraevViro"); break;
        case ReginaPrefSet::CellularInfo:
            settings.setValue("InitialAlgebraTab", "CellularInfo"); break;
        default:
            settings.setValue("InitialAlgebraTab", "Homology"); break;
    }

    settings.setValue("SurfacePropsThreshold", triSurfacePropsThreshold);
    settings.endGroup();

    settings.beginGroup("Extensions");
    settings.setValue("GAPExec", triGAPExec);
    settings.setValue("GraphvizExec", triGraphvizExec);
    settings.endGroup();

    savePythonLibraries();

    settings.beginGroup("Window");
    settings.setValue("MainSize", windowMainSize);
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    settings.remove("");
    for (int i = 0; i < fileRecent_.count(); ++i)
        settings.setValue(QString("File%1").arg(i + 1),
            fileRecent_[i].toLocalFile());
    settings.endGroup();
}

