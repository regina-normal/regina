
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

#import "SnapPeaViewController.h"
#import "SnapPeaAlgebra.h"
#import "TextHelper.h"
#import "Tri3Algebra.h"
#import "snappea/snappeatriangulation.h"

@interface SnapPeaAlgebra ()
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;

@property (weak, nonatomic) IBOutlet UILabel *filledHomologyLabel;
@property (weak, nonatomic) IBOutlet UILabel *filledHomology;
@property (weak, nonatomic) IBOutlet UILabel *filledFundLabel;
@property (weak, nonatomic) IBOutlet UILabel *filledFundName;
@property (weak, nonatomic) IBOutlet UILabel *filledFundGens;
@property (weak, nonatomic) IBOutlet UILabel *filledFundRels;
@property (weak, nonatomic) IBOutlet UITextView *filledFundDetails;

@property (weak, nonatomic) IBOutlet UILabel *unfilledHomologyLabel;
@property (weak, nonatomic) IBOutlet UILabel *unfilledHomology;
@property (weak, nonatomic) IBOutlet UILabel *unfilledFundLabel;
@property (weak, nonatomic) IBOutlet UILabel *unfilledFundName;
@property (weak, nonatomic) IBOutlet UILabel *unfilledFundGens;
@property (weak, nonatomic) IBOutlet UILabel *unfilledFundRels;
@property (weak, nonatomic) IBOutlet UITextView *unfilledFundDetails;

@property (strong, nonatomic) SnapPeaViewController* viewer;
@property (assign, nonatomic) regina::SnapPeaTriangulation* packet;
@end

@implementation SnapPeaAlgebra

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<SnapPeaViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;
    [self reloadPacket];
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.header volume:self.volume solnType:self.solnType];

    if (self.packet->isNull() || self.packet->countFilledCusps() == 0) {
        self.filledHomologyLabel.hidden = YES;
        self.filledHomology.hidden = YES;
        self.filledFundLabel.hidden = YES;
        self.filledFundName.hidden = YES;
        self.filledFundGens.hidden = YES;
        self.filledFundRels.hidden = YES;
        self.filledFundDetails.hidden = YES;
    } else {
        self.filledHomologyLabel.hidden = NO;
        self.filledHomology.hidden = NO;
        self.filledFundLabel.hidden = NO;
        self.filledFundName.hidden = NO;
        self.filledFundGens.hidden = NO;
        self.filledFundRels.hidden = NO;
        self.filledFundDetails.hidden = NO;

        const regina::AbelianGroup* h1 = self.packet->homologyFilled();
        if (h1)
            self.filledHomology.text = @(h1->utf8().c_str());
        else
            self.filledHomology.attributedText = [TextHelper dimString:@"Could not compute"];

        const regina::GroupPresentation* pi1 = self.packet->fundamentalGroupFilled();
        if (pi1)
            [Tri3Algebra reloadGroup:*pi1
                               name:self.filledFundName
                               gens:self.filledFundGens
                               rels:self.filledFundRels
                            details:self.filledFundDetails];
        else {
            self.filledFundName.attributedText = [TextHelper dimString:@"Could not compute"];
            self.filledFundGens.text = self.filledFundRels.text = self.filledFundDetails.text = nil;
        }
    }

    if (self.packet->isNull()) {
        self.unfilledHomologyLabel.hidden = YES;
        self.unfilledHomology.hidden = YES;
        self.unfilledFundLabel.hidden = YES;
        self.unfilledFundName.hidden = YES;
        self.unfilledFundGens.hidden = YES;
        self.unfilledFundRels.hidden = YES;
        self.unfilledFundDetails.hidden = YES;
    } else {
        self.unfilledHomologyLabel.hidden = NO;
        self.unfilledHomology.hidden = NO;
        self.unfilledFundLabel.hidden = NO;
        self.unfilledFundName.hidden = NO;
        self.unfilledFundGens.hidden = NO;
        self.unfilledFundRels.hidden = NO;
        self.unfilledFundDetails.hidden = NO;

        self.unfilledHomology.text = @(self.packet->homology().utf8().c_str());
        [Tri3Algebra reloadGroup:self.packet->fundamentalGroup()
                           name:self.unfilledFundName
                           gens:self.unfilledFundGens
                           rels:self.unfilledFundRels
                        details:self.unfilledFundDetails];
    }
}

@end
