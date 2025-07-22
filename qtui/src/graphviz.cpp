
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

#ifdef LIBGVC_FOUND
#include "gvc.h"
#include "utilities/typeutils.h"
#include <QSvgWidget>
#endif

#ifdef LIBGVC_FOUND
// Define LIBGVC_DYNAMIC_PLUGINS if you wish to load plugins dynamically.
// This requires (amongst other things) the presence of the file config6,
// which lists all available plugins.
// #define LIBGVC_DYNAMIC_PLUGINS 1

#ifndef LIBGVC_DYNAMIC_PLUGINS
#if defined(_WIN32)
__declspec(dllimport) gvplugin_library_t gvplugin_neato_layout_LTX_library;
__declspec(dllimport) gvplugin_library_t gvplugin_dot_layout_LTX_library;
__declspec(dllimport) gvplugin_library_t gvplugin_core_LTX_library;
#else
extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;
#endif

lt_symlist_t link_lt_preloaded_symbols[] = {
    { "gvplugin_neato_layout_LTX_library", &gvplugin_neato_layout_LTX_library },
    { "gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library },
    { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library },
    { nullptr, nullptr }
};
#endif

void Graphviz::render(QSvgWidget* widget, const std::string& dot,
        Renderer renderer) {
    // At some point (Graphviz 13), Graphviz changed the type of the svgLen
    // argument to gvRenderData() from unsigned* to size_t*.  Instead of
    // trying to fix this via conditional compilation, we ask the compiler
    // to deduce the type automatically.
    using SVGLen = std::remove_pointer_t<
        regina::CallableArg<decltype(&gvRenderData), 4>::type>;
    static_assert(std::is_integral_v<SVGLen>);

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

