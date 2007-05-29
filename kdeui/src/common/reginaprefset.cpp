
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

#include "surfaces/nnormalsurfacelist.h"
#include "utilities/stringutils.h"

#include "reginaprefset.h"
#include "shortrunner.h"

#include <fstream>
#include <kstandarddirs.h>
#include <qdir.h>
#include <qfileinfo.h>

namespace {
    std::string INACTIVE("## INACTIVE ##");
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

const char* ReginaPrefSet::defaultGAPExec = "gap";
const char* ReginaPrefSet::defaultGraphvizExec = "neato";

// No need to initialise these, since the cache is only used when the
// given executable matches cacheGraphvizExec (which begins life as null).
QMutex GraphvizStatus::cacheGraphvizMutex;
QString GraphvizStatus::cacheGraphvizExec;
QString GraphvizStatus::cacheGraphvizExecFull;
GraphvizStatus GraphvizStatus::cacheGraphvizStatus;

GraphvizStatus GraphvizStatus::status(const QString& userExec,
        QString& fullExec, bool forceRecheck) {
    QMutexLocker lock(&cacheGraphvizMutex);

    if ((! forceRecheck) && cacheGraphvizStatus != unknown &&
            userExec == cacheGraphvizExec) {
        fullExec = cacheGraphvizExecFull;
        return cacheGraphvizStatus;
    }

    // We need a full requery.
    if (userExec.find("/") < 0) {
        // Hunt on the search path.
        fullExec = KStandardDirs::findExe(userExec);
        if (fullExec.isNull())
            return notFound;
    } else
        fullExec = QFileInfo(userExec).absFilePath();

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

    if (output.find("version 1.") >= 0) {
        // Only test for "dot", not "/dot".  I'd rather not get tripped
        // up with alternate path separators, and this still
        // distinguishes between the different 1.x graph drawing tools.
        if (userExec.endsWith("dot", false))
            return version1;
        return version1NotDot;
    } else if (output.find("version 0.") >= 0)
        return unsupported;
    else if (output.find("version") >= 0) {
        // Assume any other version is >= 2.x.
        return version2;
    } else {
        // Could not find a version string at all.
        return unsupported;
    }
}

ReginaPrefSet::ReginaPrefSet() :
        autoDock(true),
        autoFileExtension(true),
        censusFiles(defaultCensusFiles()),
        displayIcon(true),
        displayTagsInTree(false),
        pythonAutoIndent(true),
        pythonSpacesPerTab(4),
        pythonWordWrap(false),
        snapPeaClosed(false),
        surfacesCreationCoords(regina::NNormalSurfaceList::STANDARD),
        treeJumpSize(10),
        triEditMode(DirectEdit),
        triGAPExec(defaultGAPExec),
        triGraphvizExec(defaultGraphvizExec),
        triInitialTab(Gluings),
        triInitialSkeletonTab(SkelComp),
        triInitialAlgebraTab(Homology),
        triSurfacePropsThreshold(6) {
}

ReginaFilePrefList ReginaPrefSet::defaultCensusFiles() {
    ReginaFilePrefList ans;

    ans.push_back(ReginaFilePref(REGINA_DATADIR
        "/examples/closed-or-census.rga"));
    ans.push_back(ReginaFilePref(REGINA_DATADIR
        "/examples/closed-nor-census.rga"));
    ans.push_back(ReginaFilePref(REGINA_DATADIR
        "/examples/knot-link-census.rga"));
    ans.push_back(ReginaFilePref(REGINA_DATADIR
        "/examples/snappea-census.rga"));
    ans.push_back(ReginaFilePref(REGINA_DATADIR
        "/examples/closed-hyp-census.rga"));

    return ans;
}

QString ReginaPrefSet::pythonLibrariesConfig() {
    return QDir::homeDirPath() + "/.regina-libs";
}

bool ReginaPrefSet::readPythonLibraries() {
    pythonLibraries.clear();

    std::ifstream in(pythonLibrariesConfig().ascii());
    if (! in)
        return false;

    bool active;
    std::string line;
    while (! in.eof()) {
        line.clear();
        std::getline(in, line);

        // Is the file inactive?
        active = true;
        if (regina::startsWith(line, INACTIVE)) {
            active = false;
            line = line.substr(INACTIVE.length());
        }

        line = regina::stripWhitespace(line);

        // Is it a file at all?
        if (line.empty() || line[0] == '#')
            continue;

        // Add the file.
        pythonLibraries.push_back(ReginaFilePref(line.c_str(), active));
    }

    return true;
}

bool ReginaPrefSet::writePythonLibraries() const {
    std::ofstream out(pythonLibrariesConfig().ascii());
    if (! out)
        return false;

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

