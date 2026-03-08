
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "graphviz.h"

// We make two passes at clearing out all of the PACKAGE_* macros,
// since both regina-config.h and graphviz_version.h provide different
// (and overlapping) combinations of these.
//
// It is better to have none of these macros than to be unsure of
// whose we're using.
//
// The list of macros that we clear is the union of what Regina and Graphviz
// provide.

// At this point we have imported Regina's PACKAGE_* macros.
#undef PACKAGE_BUGREPORT
#undef PACKAGE_BUILD_STRING
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION
#undef PACKAGE_VERSION_MAJOR
#undef PACKAGE_VERSION_MINOR

#ifdef LIBGVC_FOUND
#include "graphviz_version.h"
#include "gvc.h"
#include <QSvgWidget>
#endif

// At this point we have (possibly) imported Graphviz's PACKAGE_* macros.
#undef PACKAGE_BUGREPORT
#undef PACKAGE_BUILD_STRING
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION
#undef PACKAGE_VERSION_MAJOR
#undef PACKAGE_VERSION_MINOR

#ifdef LIBGVC_FOUND

#ifndef LIBGVC_DYNAMIC_PLUGINS
#if defined(_WIN32)
__declspec(dllimport) gvplugin_library_t gvplugin_neato_layout_LTX_library;
__declspec(dllimport) gvplugin_library_t gvplugin_dot_layout_LTX_library;
__declspec(dllimport) gvplugin_library_t gvplugin_core_LTX_library;
#else
extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;
#endif // _WIN32

lt_symlist_t link_lt_preloaded_symbols[] = {
    { "gvplugin_neato_layout_LTX_library", &gvplugin_neato_layout_LTX_library },
    { "gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library },
    { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library },
    { nullptr, nullptr }
};
#endif // LIBGVC_DYNAMIC_PLUGINS

void Graphviz::render(QSvgWidget* widget, const std::string& dot,
        Renderer renderer) {
    // In Graphviz 13.0.0, Graphviz changed the type of the svgLen argument
    // to gvRenderData() from unsigned* to size_t*.  This means we need to
    // distinguish between graphviz ≥ 13.0.0 vs ≤ 12.2.1.
    //
    // Nowadays, graphviz_version.h contains integer macros
    // GRAPHVIZ_VERSION_MAJOR, GRAPHVIZ_VERSION_MINOR, GRAPHVIZ_VERSION_PATCH.
    // However, these were not introduced until graphviz 14.0.0.
    //
    // Happily, the transition from 12.2.1 to 13.0.0 also saw the integer macro
    // GVPLUGIN_VERSION change from 6 to 8.  This gives us enough information
    // to deduce our place in the timeline.  Note that GVPLUGIN_VERSION has
    // been around for as long as the header graphviz_version.h, which was
    // introduced in graphviz 2.24.0 (2009-06-16).
    //
    #if defined(GRAPHVIZ_VERSION_MAJOR)
        // A modern graphviz, with version ≥ 14.0.0.
        using SVGLen = size_t;
    #elif defined(GVPLUGIN_VERSION)
        #if GVPLUGIN_VERSION >= 8
            // An older graphviz, with 13.0.0 ≤ version < 14.0.0.
            using SVGLen = size_t;
        #else
            // An older graphviz, with 2.24.0 ≤ version ≤ 12.2.1.
            using SVGLen = unsigned;
        #endif
    #else
        // An ancient graphviz.  Assume we have found graphviz_version.h,
        // we should never reach this branch of the code.
        #error Cannot find either GRAPHVIZ_VERSION_MAJOR or GVPLUGIN_VERSION
    #endif

    char* svg;
    SVGLen svgLen;

#ifdef LIBGVC_DYNAMIC_PLUGINS
    GVC_t* gvc = gvContext();
#else
    // Manually specify our plugins to avoid on-demand loading.
    GVC_t* gvc = gvContextPlugins(link_lt_preloaded_symbols, 0);

    gvAddLibrary(gvc, &gvplugin_core_LTX_library);
    switch (renderer) {
        case Graphviz::Renderer::Neato:
            gvAddLibrary(gvc, &gvplugin_neato_layout_LTX_library); break;
        case Graphviz::Renderer::Dot:
            gvAddLibrary(gvc, &gvplugin_dot_layout_LTX_library); break;
    }
#endif

    Agraph_t* g = agmemread(dot.c_str());
    switch (renderer) {
        case Graphviz::Renderer::Neato:
            gvLayout(gvc, g, "neato"); break;
        case Graphviz::Renderer::Dot:
            gvLayout(gvc, g, "dot"); break;
    }
    gvRenderData(gvc, g, "svg", &svg, &svgLen);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    widget->load(QByteArray(svg, svgLen));
    gvFreeRenderData(svg);
    widget->resize(widget->sizeHint());
}
#else
const char* Graphviz::notSupported =
    "<qt>This copy of <i>Regina</i> was built without "
    "<i>Graphviz</i> support.  Therefore I cannot draw graphs.<p>"
    "If you downloaded <i>Regina</i> as a ready-made package, please "
    "contact the package maintainer for a <i>Graphviz</i>-enabled build.<p>"
    "If you compiled <i>Regina</i> yourself, try installing the "
    "<i>Graphviz</i> libraries on your system and then compiling "
    "<i>Regina</i> again.</qt>";
#endif // LIBGVC_FOUND

