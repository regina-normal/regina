
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
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

#define KEY_LAST_TRI_GRAPH_TYPE @"ViewTri3GraphType"

#import "SnapPeaViewController.h"
#import "Tri3ViewController.h"
#import "Tri3Graph.h"
#import "treewidth/treedecomposition.h"
#import "triangulation/facetpairing3.h"
#import "triangulation/dim3.h"
#import "gvc.h"

extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;

@interface Tri3Graph ()
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UISegmentedControl *graphType;
@property (weak, nonatomic) IBOutlet UILabel *graphProperties;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *graphPropertiesGap;
@property (weak, nonatomic) IBOutlet UIWebView *graph;

@property (assign, nonatomic) regina::Triangulation<3>* packet;
@end

@implementation Tri3Graph

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = static_cast<regina::Triangulation<3>*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    self.graphType.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TRI_GRAPH_TYPE];

    [self reloadPacket];
}

- (IBAction)typeChanged:(id)sender {
    [self reloadPacket];
    [[NSUserDefaults standardUserDefaults] setInteger:self.graphType.selectedSegmentIndex forKey:KEY_LAST_TRI_GRAPH_TYPE];
}

- (void)reloadPacket
{
    if ([self.parentViewController isKindOfClass:[SnapPeaViewController class]])
        [static_cast<SnapPeaViewController*>(self.parentViewController) updateHeader:self.header volume:self.volume solnType:self.solnType];
    else
        [static_cast<Tri3ViewController*>(self.parentViewController) updateHeader:self.header lockIcon:self.lockIcon];

    std::string dot;
    switch (self.graphType.selectedSegmentIndex) {
        case 0: {
            regina::FacetPairing<3> p(*self.packet);
            dot = p.dot("v", false /* subgraph */, true /* labels */);
            self.graphProperties.text = nil;
            self.graphPropertiesGap.constant = 0;
            break;
        }

        case 1:
        case 2: {
            regina::TreeDecomposition t(*self.packet);
            if (self.graphType.selectedSegmentIndex == 2)
                t.makeNice();
            dot = t.dot();
            self.graphProperties.text = [NSString stringWithFormat:@"%d bags, width %d", t.size(), t.width()];
            self.graphPropertiesGap.constant = 20;
            break;
        }

        default:
            // Clear the display.
            [self.graph loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"about:blank"]]];
            self.graphProperties.text = nil;
            self.graphPropertiesGap.constant = 20;
            return;
    }

    char* svg;
    unsigned svgLen;

    GVC_t* gvc = gvContext();
    gvAddLibrary(gvc, &gvplugin_core_LTX_library);
    if (self.graphType.selectedSegmentIndex == 0)
        gvAddLibrary(gvc, &gvplugin_neato_layout_LTX_library);
    else
        gvAddLibrary(gvc, &gvplugin_dot_layout_LTX_library);
    Agraph_t* g = agmemread(dot.c_str());
    if (self.graphType.selectedSegmentIndex == 0)
        gvLayout(gvc, g, "neato");
    else
        gvLayout(gvc, g, "dot");
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
