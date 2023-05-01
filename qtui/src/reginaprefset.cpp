
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "core/engine.h"
#include "file/globaldirs.h"
#include "snappea/snappeatriangulation.h"
#include "surface/normalsurfaces.h"
#include "utilities/stringutils.h"

#include "reginaprefset.h"
#include "reginasupport.h"
#include "shortrunner.h"

#include <fstream>
#include <thread>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextStream>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QTextCodec>
#endif

namespace {
    const QString INACTIVE("## INACTIVE ##");
}

ReginaPrefSet ReginaPrefSet::instance_;

#ifdef Q_OS_WIN32
    const char* ReginaPrefSet::defaultGAPExec = "gap.exe";
#else
    const char* ReginaPrefSet::defaultGAPExec = "gap";
#endif

ReginaPrefSet::ReginaPrefSet() :
        anglesCreationTaut(false),
        fileRecentMax(10),
        displayTagsInTree(false),
        displayUnicode(true),
        fileImportExportCodec("UTF-8"),
        helpIntroOnStartup(true),
        hypersurfacesCreationCoords(regina::HS_STANDARD),
        hypersurfacesCreationList(regina::HS_LIST_DEFAULT),
        linkCodeType(Gauss),
        linkCreationType(0),
        linkCrossingsStyle(PictorialCrossings),
        linkHomflyType(HomflyAZ),
        linkInitialGraphType(TreeDecomposition),
        pythonAutoIndent(true),
        pythonSpacesPerTab(4),
        pythonWordWrap(false),
        surfacesCompatThreshold(100),
        surfacesCreationCoords(regina::NS_STANDARD),
        surfacesCreationList(regina::NS_LIST_DEFAULT),
        surfacesInitialCompat(LocalCompat),
        surfacesSupportOriented(false),
        threadCount(ThreadPolite),
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
        tabLink(0),
        tabSnapPeaTri(0),
        tabSnapPeaTriAlgebra(0),
        tabSurfaceList(0),
        triDim2CreationType(0),
        triDim3CreationType(0),
        triDim4CreationType(0),
        triGAPExec(defaultGAPExec),
        triGraphvizLabels(true),
        triInitialGraphType(DualGraph),
        triSurfacePropsThreshold(6),
        warnOnNonEmbedded(true) {
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

    QString home = QFile::decodeName(regina::GlobalDirs::home().c_str());
#if defined(REGINA_INSTALL_BUNDLE) && defined(Q_OS_MACX) && defined(REGINA_XCODE_BUNDLE)
    // The Xcode build installs the handbooks as bundle resources, at the root
    // of the Resources directory.
    QString index = home +
    QString("/docs-%1/index.html").arg(handbookName);
    QString page = home +
    QString("/docs-%1/%2.html").arg(handbookName).arg(section);
#else
    QString index = home +
        QString("/docs/en/%1/index.html").arg(handbookName);
    QString page = home +
        QString("/docs/en/%1/%2.html").arg(handbookName).arg(section);
#endif
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
        // Fall back to the version of the handbook on the website.
        if (! QDesktopServices::openUrl(QUrl(QString(
                "https://regina-normal.github.io/" PACKAGE_VERSION
                "/docs/%1/%2.html").arg(handbookName).arg(section)))) {
            ReginaSupport::warn(parentWidget,
                QObject::tr("I could not find the requested documentation."),
                QObject::tr("<qt>It should be installed at: "
                    "<tt>%1</tt><p>Please mail the authors for "
                    "assistance.</qt>").arg(index.toHtmlEscaped()));
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

    settings.beginGroup("Compute");
    QString str = settings.value("ThreadCount").toString();
    if (str == "Single")
        threadCount = ReginaPrefSet::ThreadSingle;
    else if (str == "All")
        threadCount = ReginaPrefSet::ThreadAll;
    else
        threadCount = ReginaPrefSet::ThreadPolite; /* default */
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

    settings.beginGroup("Link");
    linkCreationType = settings.value("CreationType", 0).toInt();
    str = settings.value("CodeType").toString();
    if (str == "DowkerThistlethwaite")
        linkCodeType = ReginaPrefSet::DowkerThistlethwaite;
    else if (str == "KnotSig")
        linkCodeType = ReginaPrefSet::KnotSig;
    else if (str == "PlanarDiagram")
        linkCodeType = ReginaPrefSet::PlanarDiagram;
    else if (str == "Jenkins")
        linkCodeType = ReginaPrefSet::Jenkins;
    else
        linkCodeType = ReginaPrefSet::Gauss; /* default */
    str = settings.value("CrossingsStyle").toString();
    if (str == "Text")
        linkCrossingsStyle = ReginaPrefSet::TextCrossings;
    else
        linkCrossingsStyle = ReginaPrefSet::PictorialCrossings; /* default */
    str = settings.value("HomflyType").toString();
    if (str == "LM")
        linkHomflyType = ReginaPrefSet::HomflyLM;
    else
        linkHomflyType = ReginaPrefSet::HomflyAZ; /* default */
    str = settings.value("InitialGraphType").toString();
    if (str == "NiceTree")
        linkInitialGraphType = ReginaPrefSet::NiceTreeDecomposition;
    else
        linkInitialGraphType = ReginaPrefSet::TreeDecomposition; /* default */
    settings.endGroup();

    settings.beginGroup("Python");
    pythonAutoIndent = settings.value("AutoIndent", true).toBool();
    pythonSpacesPerTab = settings.value("SpacesPerTab", 4).toInt();
    pythonWordWrap = settings.value("WordWrap", false).toBool();
    settings.endGroup();

    settings.beginGroup("SnapPea");
    regina::SnapPeaTriangulation::enableKernelMessages(
        settings.value("KernelMessages", false).toBool());
    settings.endGroup();

    settings.beginGroup("Surfaces");
    surfacesCompatThreshold = settings.value(
        "CompatibilityThreshold", 100).toInt();
    surfacesCreationCoords = static_cast<regina::NormalCoords>(settings.value(
        "CreationCoordinates", regina::NS_STANDARD).toInt());
    surfacesCreationList = regina::NormalList::fromInt(settings.value(
        "CreationList", regina::NS_LIST_DEFAULT).toInt());

    str = settings.value("InitialCompat").toString();
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
    tabDim2Tri = settings.value("Dim2Tri", 0).toInt();
    tabDim2TriSkeleton = settings.value("Tri2Skeleton", 0).toInt();
    tabDim3Tri = settings.value("Dim3Tri", 0).toInt();
    tabDim3TriAlgebra = settings.value("Dim3TriAlgebra", 0).toInt();
    tabDim3TriSkeleton = settings.value("Dim3TriSkeleton", 0).toInt();
    tabDim4Tri = settings.value("Dim4Tri", 0).toInt();
    tabDim4TriAlgebra = settings.value("Tri4Algebra", 0).toInt();
    tabDim4TriSkeleton = settings.value("Tri4Skeleton", 0).toInt();
    tabHypersurfaceList = settings.value("HypersurfaceList", 0).toInt();
    tabLink = settings.value("Link", 0).toInt();
    tabSnapPeaTri = settings.value("SnapPeaTri", 0).toInt();
    tabSnapPeaTriAlgebra = settings.value("SnapPeaTriAlgebra", 0).toInt();
    tabSurfaceList = settings.value("SurfaceList", 0).toInt();
    settings.endGroup();

    settings.beginGroup("Triangulation");
    triDim2CreationType = settings.value("Dim2CreationType", 0).toInt();
    triDim3CreationType = settings.value("Dim3CreationType", 0).toInt();
    triDim4CreationType = settings.value("Dim4CreationType", 0).toInt();
    triGraphvizLabels = settings.value("GraphvizLabels", true).toBool();

    str = settings.value("InitialGraphType").toString();
    if (str == "Tree")
        triInitialGraphType = ReginaPrefSet::TreeDecomposition;
    else if (str == "NiceTree")
        triInitialGraphType = ReginaPrefSet::NiceTreeDecomposition;
    else
        triInitialGraphType = ReginaPrefSet::DualGraph; /* default */

    triSurfacePropsThreshold = settings.value(
        "SurfacePropsThreshold", 6).toInt();
    settings.endGroup();

    settings.beginGroup("Tools");
    triGAPExec = settings.value("GAPExec", defaultGAPExec).toString().trimmed();
    settings.endGroup();

    settings.beginGroup("Window");
    windowMainSize = settings.value("MainSizeV4", QSize()).toSize();
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

    settings.beginGroup("Compute");
    switch (threadCount) {
        case ReginaPrefSet::ThreadSingle:
            settings.setValue("ThreadCount", "Single"); break;
        case ReginaPrefSet::ThreadAll:
            settings.setValue("ThreadCount", "All"); break;
        default:
            settings.setValue("ThreadCount", "Polite"); break;
    }
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

    settings.beginGroup("Link");
    settings.setValue("CreationType", linkCreationType);
    switch (linkCodeType) {
        case ReginaPrefSet::DowkerThistlethwaite:
            settings.setValue("CodeType", "DowkerThistlethwaite"); break;
        case ReginaPrefSet::KnotSig:
            settings.setValue("CodeType", "KnotSig"); break;
        case ReginaPrefSet::PlanarDiagram:
            settings.setValue("CodeType", "PlanarDiagram"); break;
        case ReginaPrefSet::Jenkins:
            settings.setValue("CodeType", "Jenkins"); break;
        default:
            settings.setValue("CodeType", "Gauss"); break;
    }
    switch (linkCrossingsStyle) {
        case ReginaPrefSet::TextCrossings:
            settings.setValue("CrossingsStyle", "Text"); break;
        default:
            settings.setValue("CrossingsStyle", "Pictorial"); break;
    }
    switch (linkHomflyType) {
        case ReginaPrefSet::HomflyLM:
            settings.setValue("HomflyType", "LM"); break;
        default:
            settings.setValue("HomflyType", "AZ"); break;
    }
    switch (linkInitialGraphType) {
        case ReginaPrefSet::NiceTreeDecomposition:
            settings.setValue("InitialGraphType", "NiceTree"); break;
        default:
            settings.setValue("InitialGraphType", "Tree"); break;
    }
    settings.endGroup();

    settings.beginGroup("Python");
    settings.setValue("AutoIndent", pythonAutoIndent);
    settings.setValue("SpacesPerTab", pythonSpacesPerTab);
    settings.setValue("WordWrap", pythonWordWrap);
    settings.endGroup();

    settings.beginGroup("SnapPea");
    settings.setValue("KernelMessages",
        regina::SnapPeaTriangulation::kernelMessagesEnabled());
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
    settings.setValue("Tri2Skeleton", tabDim2TriSkeleton);
    settings.setValue("Dim3Tri", tabDim3Tri);
    settings.setValue("Dim3TriAlgebra", tabDim3TriAlgebra);
    settings.setValue("Dim3TriSkeleton", tabDim3TriSkeleton);
    settings.setValue("Dim4Tri", tabDim4Tri);
    settings.setValue("Link", tabLink);
    settings.setValue("Tri4Algebra", tabDim4TriAlgebra);
    settings.setValue("Tri4Skeleton", tabDim4TriSkeleton);
    settings.setValue("HypersurfaceList", tabHypersurfaceList);
    settings.setValue("SnapPeaTri", tabSnapPeaTri);
    settings.setValue("SnapPeaTriAlgebra", tabSnapPeaTriAlgebra);
    settings.setValue("SurfaceList", tabSurfaceList);
    settings.endGroup();

    settings.beginGroup("Tree");
    settings.setValue("JumpSize", treeJumpSize);
    settings.endGroup();

    settings.beginGroup("Triangulation");
    settings.setValue("Dim2CreationType", triDim2CreationType);
    settings.setValue("Dim3CreationType", triDim3CreationType);
    settings.setValue("Dim4CreationType", triDim4CreationType);
    settings.setValue("GraphvizLabels", triGraphvizLabels);

    switch (triInitialGraphType) {
        case ReginaPrefSet::TreeDecomposition:
            settings.setValue("InitialGraphType", "Tree"); break;
        case ReginaPrefSet::NiceTreeDecomposition:
            settings.setValue("InitialGraphType", "NiceTree"); break;
        default:
            settings.setValue("InitialGraphType", "Dual"); break;
    }

    settings.setValue("SurfacePropsThreshold", triSurfacePropsThreshold);
    settings.endGroup();

    settings.beginGroup("Tools");
    settings.setValue("GAPExec", triGAPExec);
    settings.endGroup();

    settings.beginGroup("Window");
    settings.setValue("MainSizeV4", windowMainSize);
    settings.setValue("PythonSize", windowPythonSize);
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    settings.remove("");
    for (int i = 0; i < fileRecent_.count(); ++i)
        settings.setValue(QString("File%1").arg(i + 1),
            fileRecent_[i].toLocalFile());
    settings.endGroup();
}

ReginaPrefSet::Codec ReginaPrefSet::importExportCodec() {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    auto enc = QStringConverter::encodingForName(global().fileImportExportCodec);
    if (! enc)
        enc = QStringConverter::Utf8;
    return *enc;
#else
    QTextCodec* ans = QTextCodec::codecForName(global().fileImportExportCodec);
    if (! ans)
        ans = QTextCodec::codecForName("UTF-8");
    return ans;
#endif
}

int ReginaPrefSet::threads() {
    switch (global().threadCount) {
        case ThreadSingle:
            return 1;
        case ThreadAll:
        {
            int ans = std::thread::hardware_concurrency();
            return (ans >= 1 ? ans : 1);
        }
        default:
            // Use the ThreadPolite setting.
            return regina::politeThreads();
    }
}
