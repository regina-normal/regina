
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

#import "LinkViewController.h"
#import "link/link.h"

@implementation LinkViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setSelectedImages:@[[NSNull null] /* Polynomials */,
                              [NSNull null] /* Codes */,
                              @"Tab-Graph-Bold"]];
    [self registerDefaultKey:@"ViewLinkTab"];
}

- (void)updateHeader:(UILabel *)header
{
    if (self.packet->isEmpty())
        header.text = @"Empty";
    else if (self.packet->countComponents() == 1) {
        // Knot:
        if (self.packet->size() == 1)
            header.text = @"Knot with 1 crossing";
        else
            header.text = [NSString stringWithFormat:@"Knot with %zu crossings", self.packet->size()];
    } else {
        // Multiple component link:
        if (self.packet->size() == 1)
            header.text = [NSString stringWithFormat:@"Link with %zu components, 1 crossing", self.packet->countComponents()];
        else
            header.text = [NSString stringWithFormat:@"Link with %zu components, %zu crossings", self.packet->countComponents(), self.packet->size()];
    }
}

@end
