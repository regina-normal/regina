
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#import "Dim4TriangulationViewController.h"
#import "Dim4TriGraph.h"
#import "dim4/dim4facetpairing.h"
#import "dim4/dim4triangulation.h"
#import "gvc.h"

extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;

@interface Dim4TriGraph ()
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UIWebView *graph;

@property (assign, nonatomic) regina::Dim4Triangulation* packet;
@end

@implementation Dim4TriGraph

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = static_cast<regina::Dim4Triangulation*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    [self reloadPacket];
}

- (void)reloadPacket
{
    [static_cast<Dim4TriangulationViewController*>(self.parentViewController) updateHeader:self.header lockIcon:self.lockIcon];

    regina::Dim4FacetPairing* p = new regina::Dim4FacetPairing(*self.packet);
    std::string dot = p->dot("v", false /* subgraph */, true /* labels */);
    delete p;

    char* svg;
    unsigned svgLen;

    GVC_t* gvc = gvContext();
    gvAddLibrary(gvc, &gvplugin_core_LTX_library);
    gvAddLibrary(gvc, &gvplugin_neato_layout_LTX_library);
    Agraph_t* g = agmemread(dot.c_str());
    gvLayout(gvc, g, "neato");
    gvRenderData(gvc, g, "svg", &svg, &svgLen);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    // NSLog(@"SVG:\n%s", svg);

    [self.graph loadData:[NSData dataWithBytes:svg length:svgLen]
                MIMEType:@"image/svg+xml"
        textEncodingName:@"utf-8"
                 baseURL:nil];

    self.graph.scalesPageToFit = YES;
}

@end
