
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
#import "LinkCodes.h"
#import "link/link.h"

#define KEY_LINK_CODE_TYPE @"LinkCodeType"

@interface LinkCodes () {
    UIFont* codeFont;
}

@property (weak, nonatomic) IBOutlet UILabel *header;

@property (weak, nonatomic) IBOutlet UITextView *code;
@property (weak, nonatomic) IBOutlet UISegmentedControl *codeType;

@property (assign, nonatomic) regina::Link* packet;
@end

@implementation LinkCodes

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    codeFont = [UIFont fontWithName:@"Menlo" size:self.header.font.pointSize];

    self.codeType.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LINK_CODE_TYPE];
    
    self.packet = static_cast<regina::Link*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    [self reloadPacket];
}

- (void)reloadPacket
{
    [static_cast<LinkViewController*>(self.parentViewController) updateHeader:self.header];

    switch (self.codeType.selectedSegmentIndex) {
        case 0:
            if (self.packet->countComponents() == 1)
                self.code.text = @(self.packet->orientedGauss().c_str());
            else {
                self.code.text = @"Oriented Gauss codes are currently only available for knots.";
                self.code.font = self.header.font;
                return; // to avoid resetting the font below.
            }
            break;
        case 1:
            self.code.text = @(self.packet->jenkins().c_str());
            break;
    }
    
    self.code.font = codeFont;
}

- (IBAction)codeTypeChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setInteger:self.codeType.selectedSegmentIndex forKey:KEY_LINK_CODE_TYPE];
    [self reloadPacket];
}

@end
