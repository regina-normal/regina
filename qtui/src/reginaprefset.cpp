
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

#ifdef Q_OS_WIN32
    const char* ReginaPrefSet::defaultGAPExec = "gap.exe";
    const char* ReginaPrefSet::defaultGraphvizExec = "neato.exe";
#else
    const char* ReginaPrefSet::defaultGAPExec = "gap";
    const char* ReginaPrefSet::defaultGraphvizExec = "neato";
#endif

// No need to initialise these, since the cache is only used when the
// given executable matches cacheGraphvizExec (which begins life as null).
QMutex GraphvizStatus::cacheGraphvizMutex;
QString GraphvizStatus::cacheGraphvizExec;
QString GraphvizStatus::cacheGraphvizExecFull;
GraphvizStatus GraphvizStatus::cacheGraphvizStatus;

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
        switch ((*it)[0].toAscii()) {
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
#ifdef Q_OS_WIN32
        // Windows uses a different separator in $PATH
        QString pathSeparator = ";";
#else
        QString pathSeparator = ":";
#endif
        QStringList pathList = paths.split(pathSeparator);

        // Add the "usual" location of Graphviz to the search path.
#ifdef Q_OS_MACX
        pathList.push_back("/usr/local/bin");
#endif

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
        displayTagsInTree(false),
        fileImportExportCodec("UTF-8"),
        fileRecentMax(10),
        helpIntroOnStartup(true),
        pythonAutoIndent(true),
        pythonSpacesPerTab(4),
        pythonWordWrap(false),
        snapPeaClosed(false),
        surfacesCompatThreshold(100),
        surfacesCreationCoords(regina::NNormalSurfaceList::STANDARD),
        surfacesInitialCompat(LocalCompat),
        surfacesInitialTab(Summary),
        surfacesSupportOriented(false),
        treeJumpSize(10),
        triGAPExec(defaultGAPExec),
        triGraphvizExec(defaultGraphvizExec),
        triGraphvizLabels(true),
        triInitialTab(Gluings),
        triInitialSkeletonTab(SkelComp),
        triInitialAlgebraTab(Homology),
        triSurfacePropsThreshold(6),
        useDock(false),
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
    useDock = settings.value("UseDock", false).toBool();
    displayTagsInTree = settings.value("DisplayTagsInTree", false).toBool();
    settings.endGroup();

    settings.beginGroup("Census");
    censusFiles.clear();
    // System census files:
    QString exDir = QFile::decodeName(regina::NGlobalDirs::examples().c_str());
    censusFiles.push_back(ReginaFilePref(exDir + "/closed-or-census.rga",
        tr("Closed orientable census"), "ClosedOr", settings));
    censusFiles.push_back(ReginaFilePref(exDir + "/closed-nor-census.rga",
        tr("Closed non-orientable census"), "ClosedNor", settings));
    censusFiles.push_back(ReginaFilePref(exDir + "/knot-link-census.rga",
        tr("Hyperbolic knot/link census"), "KnotLink", settings));
    censusFiles.push_back(ReginaFilePref(exDir + "/snappea-census.rga",
        tr("Cusped hyperbolic census"), "CuspedHyp", settings));
    censusFiles.push_back(ReginaFilePref(exDir + "/closed-hyp-census.rga",
        tr("Closed hyperbolic census"), "ClosedHyp", settings));
    // Additional user census files:
    ReginaFilePref::readUserKey(censusFiles, settings);
    settings.endGroup();
    
    settings.beginGroup("File");
    fileRecentMax = settings.value("RecentMax", 10).toInt();
    fileImportExportCodec = settings.value("ImportExportCodec",
        QByteArray("UTF-8")).toByteArray();
    settings.endGroup();

    settings.beginGroup("Help");
    helpIntroOnStartup = settings.value("IntroOnStartup", true).toBool();
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

    surfacesSupportOriented = settings.value("SupportOriented", false).toBool();
    settings.endGroup();

    settings.beginGroup("Tree");
    treeJumpSize = settings.value("JumpSize", 10).toInt();
    settings.endGroup();

    settings.beginGroup("Triangulation");
    triGraphvizLabels = settings.value("GraphvizLabels", true).toBool();

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

    settings.beginGroup("Tools");
    pdfExternalViewer = settings.value("PDFViewer").toString().trimmed();
    triGAPExec = settings.value("GAPExec", defaultGAPExec).toString().trimmed();
    triGraphvizExec = settings.value("GraphvizExec", defaultGraphvizExec).
        toString().trimmed();
    settings.endGroup();

    readPythonLibraries();

    settings.beginGroup("Window");
    windowMainSize = settings.value("MainSize", QSize()).toSize();
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
    settings.beginGroup("Display");
    // Save the current set of preferences.
    settings.setValue("UseDock", useDock);
    settings.setValue("DisplayTagsInTree", displayTagsInTree);
    settings.endGroup();

    settings.beginGroup("Census");
    ReginaFilePref::writeKeys(censusFiles, settings);
    settings.endGroup();

    settings.beginGroup("File");
    settings.setValue("RecentMax", fileRecentMax);
    settings.setValue("ImportExportCodec", fileImportExportCodec);
    settings.endGroup();

    settings.beginGroup("Help");
    settings.setValue("IntroOnStartup", helpIntroOnStartup);
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

    settings.setValue("SupportOriented", surfacesSupportOriented);
    settings.endGroup();

    settings.beginGroup("Tree");
    settings.setValue("JumpSize", treeJumpSize);
    settings.endGroup();

    settings.beginGroup("Triangulation");
    settings.setValue("GraphvizLabels", triGraphvizLabels);

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

    settings.beginGroup("Tools");
    settings.setValue("PDFViewer", pdfExternalViewer);
    settings.setValue("GAPExec", triGAPExec);
    settings.setValue("GraphvizExec", triGraphvizExec);
    settings.endGroup();

    savePythonLibraries();

    settings.beginGroup("Window");
    settings.setValue("MainSize", windowMainSize);
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

