
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include <cstdlib> // for getenv()
#include <cstring> // for strdup()
#include <libgen.h> // for dirname()
#include <unistd.h> // for access()
#include "regina-config.h"
#include "file/globaldirs.h"

// We will be special-casing the Xcode build here.
#if defined(REGINA_INSTALL_BUNDLE) && ! defined(REGINA_XCODE_BUNDLE)
    #error "Regina only supports macOS bundles through the Xcode build."
#elif defined(REGINA_XCODE_BUNDLE) && ! defined(REGINA_INSTALL_BUNDLE)
    #error "The Xcode build must be configured as REGINA_INSTALL_BUNDLE."
#endif

namespace regina {

std::string GlobalDirs::home_(REGINA_DATADIR);
std::string GlobalDirs::pythonModule_(REGINA_PYLIBDIR);
std::string GlobalDirs::census_(REGINA_DATADIR "/data/census");
std::string GlobalDirs::engineDocs_(REGINA_DATADIR "/engine-docs");

void GlobalDirs::setDirs(const std::string& homeDir,
        const std::string& pythonModuleDir,
        const std::string& censusDir) {
    if (! homeDir.empty()) {
        home_ = homeDir;
        engineDocs_ = home_ + "/engine-docs";
    }

    // The empty string has an explicit meaning for pythonModule_.
    pythonModule_ = pythonModuleDir;

    if (! censusDir.empty())
        census_ = censusDir;
}

void GlobalDirs::deduceDirs(const char* executable) {
    // Make a copy of executable, since dirname() might modify it.
    char* copy = strdup(executable);
    std::string exeDir = dirname(copy);
    free(copy);

    const char* env;

#if defined(REGINA_XCODE_BUNDLE)
    // Are we running from within Xcode?
    bool inXcode = false;
    std::string builtProducts;
    env = std::getenv("__XCODE_BUILT_PRODUCTS_DIR_PATHS");
    if (env && *env) {
        builtProducts = env;
        inXcode = true;
    }
#else
    // Are we in the cmake build tree?
    bool inCMakeBuildTree = true;
    std::string buildRoot = exeDir;
    std::string sourceRoot;
    while (true) {
        // A file that should exist in every build subdirectory:
        if (access((buildRoot + "/cmake_install.cmake").c_str(), F_OK) != 0) {
            inCMakeBuildTree = false;
            break;
        }

        // A file that should exist in only in the build root:
        if (access((buildRoot + "/CMakeCache.txt").c_str(), F_OK) == 0) {
            // Success!
            // Now locate the source root, which we assume is either
            // buildRoot or its immediate parent.
            if (access((buildRoot + "/CMakeLists.txt").c_str(), F_OK) == 0)
                sourceRoot = buildRoot;
            else if (access((buildRoot + "/../CMakeLists.txt").c_str(),
                    F_OK) == 0)
                sourceRoot = buildRoot + "/..";
            else {
                inCMakeBuildTree = false;
                break;
            }

            if (access((sourceRoot + "/engine/regina-config.h.in").c_str(),
                    F_OK) != 0) {
                inCMakeBuildTree = false;
                break;
            }
            break;
        }

        // At this stage it looks like we are in a subdirectory within
        // the build tree, but we have not yet found the build root.
        buildRoot += "/..";
    }
#endif

    env = std::getenv("REGINA_HOME");
    if (env && *env) {
        home_ = env;
        census_ = home_ + "/data/census";
        engineDocs_ = home_ + "/engine-docs";
    }
#if defined(REGINA_XCODE_BUNDLE)
    else {
        home_ = exeDir + "/../Resources";
        engineDocs_ = home_ + "/engine-docs";
        if (inXcode) {
            // Make sure we can find the census databases where they were first
            // built, even if we have not yet constructed a full app bundle.
            // (This matters, for instance, when running the test suite.)
            census_ = builtProducts + "/..";
        } else {
            // The xcode-built MacOS bundle puts the databases in the root
            // resources directory: Regina.app/Contents/Resources.
            // This is because the databases are "derived sources" and so must
            // be installed via "copy bundle resources" (not "copy files").
            census_ = home_;
        }
    }
#elif defined(REGINA_INSTALL_WINDOWS)
    else if (inCMakeBuildTree) {
        home_ = sourceRoot;
        census_ = buildRoot + "/engine/data/census";
        engineDocs_ = buildRoot + "/docs/engine";
    } else {
        // The Windows installation tries to mirror the XDG installation.
        home_ = exeDir + "\\..\\share\\regina";
        census_ = home_ + "\\data\\census";
        engineDocs_ = home_ + "\\engine-docs";
    }
#else
    else if (inCMakeBuildTree) {
        home_ = sourceRoot;
        census_ = buildRoot + "/engine/data/census";
        engineDocs_ = buildRoot + "/docs/engine";
    }
    // If we are not in the build tree then this should be a standard
    // XDG installation, and we should be able to rely on the hard-coded paths
    // that were set at build time.
#endif

    env = std::getenv("REGINA_PYLIBDIR");
    if (env && *env) {
        pythonModule_ = env;
    }
#if defined(REGINA_INSTALL_BUNDLE)
    else {
        // Location inside the macOS app bundle:
        pythonModule_ = exeDir + "/python";
    }
#elif defined(REGINA_INSTALL_WINDOWS)
    else if (inCMakeBuildTree) {
        pythonModule_ = buildRoot + "/python";
    } else {
        // Again, the Windows installation follows the XDG installation.
        pythonModule_ = exeDir + "\\..\\lib\\regina\\python";
    }
#else
    else if (inCMakeBuildTree) {
        pythonModule_ = buildRoot + "/python";
    }
    // As before, if we are not in the built tree then use the hard-coded paths.
#endif
}

} // namespace regina

