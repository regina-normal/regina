
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

#import "TriangulationViewController.h"
#import "TriRecognitionViewController.h"
#import "census/ncensus.h"
#import "triangulation/ntriangulation.h"

@interface TriRecognitionViewController ()
@property (weak, nonatomic) IBOutlet UITextView *detail;

@property (strong, nonatomic) TriangulationViewController* viewer;
@property (assign, nonatomic) regina::NTriangulation* packet;
@end

@implementation TriRecognitionViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    // Display the results of a census lookup.
    std::string display;
    
    regina::NCensusHits* hits = regina::NCensus::lookup(static_cast<regina::NTriangulation*>(self.packet)->isoSig());
    if (hits->count() == 0)
        display = "Census: Not found";
    else {
        display = "Census: ";
        const regina::NCensusHit* hit;
        for (hit = hits->first(); hit; hit = hit->next()) {
            if (hit != hits->first())
                display += ", ";
            display += hit->name();
        }
    }
    delete hits;
    
    display += "\n\n";
    display += self.packet->detail();
    
    _detail.text = [NSString stringWithUTF8String:display.c_str()];
}

@end
