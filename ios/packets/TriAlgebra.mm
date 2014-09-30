
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

#import "TextHelper.h"
#import "TriangulationViewController.h"
#import "TriAlgebra.h"
#import "triangulation/ntriangulation.h"

@interface TriAlgebra ()
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *h1;
@property (weak, nonatomic) IBOutlet UILabel *h1Rel;
@property (weak, nonatomic) IBOutlet UILabel *h1Bdry;
@property (weak, nonatomic) IBOutlet UILabel *h2;
@property (weak, nonatomic) IBOutlet UILabel *h2z2;
@property (weak, nonatomic) IBOutlet UILabel *fundName;
@property (weak, nonatomic) IBOutlet UILabel *fundGens;
@property (weak, nonatomic) IBOutlet UILabel *fundRels;
@property (weak, nonatomic) IBOutlet UITextView *fundRelsDetails;
@property (weak, nonatomic) IBOutlet UITextField *tvArgs;
@property (weak, nonatomic) IBOutlet UITableView *tvValues;

@property (strong, nonatomic) TriangulationViewController* viewer;
@property (assign, nonatomic) regina::NTriangulation* packet;
@end

@implementation TriAlgebra

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<TriangulationViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    [self.viewer updateHeader:self.header];

    // Simplify before doing any algebraic computations.
    regina::NTriangulation t(*self.packet);
    t.intelligentSimplify();

    self.h1.text = @(t.getHomologyH1().str().c_str());

    if (self.packet->isValid()) {
        self.h1Rel.text = @(t.getHomologyH1Rel().str().c_str());
        self.h1Bdry.text = @(t.getHomologyH1Bdry().str().c_str());
        self.h2.text = @(t.getHomologyH2().str().c_str());

        unsigned long coeffZ2 = t.getHomologyH2Z2();
        if (coeffZ2 == 0)
            self.h2z2.text = @"0";
        else if (coeffZ2 == 1)
            self.h2z2.text = @"Z_2";
        else
            self.h2z2.text = [NSString stringWithFormat:@"%ld Z_2", coeffZ2];
    } else {
        self.h1Rel.text = self.h1Bdry.text = self.h2.text = self.h2z2.text = @"Invalid";
    }

    if (t.getNumberOfComponents() > 1) {
        self.fundName.text = @"Disconnected";
        self.fundGens.text = self.fundRels.text = self.fundRelsDetails.text = @"";
    } else {
        const regina::NGroupPresentation& fg = t.getFundamentalGroup();

        std::string name = fg.recogniseGroup();
        if (name.length()) {
            self.fundName.text = [@(name.c_str()) stringByReplacingOccurrencesOfString:@"↦" withString:@"→"];
        } else {
            self.fundName.attributedText = [TextHelper dimString:@"Not recognised"];
        }

        unsigned long nGens = fg.getNumberOfGenerators();
        bool alphabetic = (nGens <= 26);
        if (nGens == 0)
            self.fundGens.text = @"No generators";
        else if (nGens == 1)
            self.fundGens.text = @"1 generator: a";
        else if (nGens == 2)
            self.fundGens.text = @"2 generators: a, b";
        else if (alphabetic)
            self.fundGens.text = [NSString stringWithFormat:@"%ld generators: a ... %c",
                                  nGens, char('a' + nGens - 1)];
        else
            self.fundGens.text = [NSString stringWithFormat:@"%ld generators: g0 ... g%ld",
                                  nGens, nGens - 1];

        unsigned long nRels = fg.getNumberOfRelations();
        if (nRels == 0) {
            self.fundRels.text = @"No relations";
        } else if (nRels == 1) {
            self.fundRels.text = @"1 relation:";
        } else {
            self.fundRels.text = [NSString stringWithFormat:@"%ld relations:", nRels];
        }

        NSMutableString* rels = [[NSMutableString alloc] init];
        if (alphabetic) {
            // Generators are a, b, ...
            for (long i = 0; i < nRels; ++i) {
                NSMutableString* rel;
                const std::list<regina::NGroupExpressionTerm>& terms(fg.getRelation(i).getTerms());
                if (terms.empty())
                    rel = [[NSMutableString alloc] initWithString:@"1"];
                else {
                    rel = [[NSMutableString alloc] initWithString:@"1 ="];
                    std::list<regina::NGroupExpressionTerm>::const_iterator it;
                    for (it = terms.begin(); it != terms.end(); ++it) {
                        [rel appendString:@" "];
                        if (it->exponent == 0)
                            [rel appendString:@"1"];
                        else {
                            [rel appendFormat:@"%c", char('a' + it->generator)];
                            if (it->exponent != 1)
                                [rel appendFormat:@"^%ld", it->exponent];
                        }
                    }
                }
                if (i > 0)
                    [rels appendString:@"\n"];
                [rels appendString:rel];
            }
        } else {
            // Generators are g0, g1, ...
            // This is the default text that comes from the calculation engine.
            for (long i = 0; i < nRels; ++i) {
                if (i > 0)
                    [rels appendString:@"\n"];
                [rels appendFormat:@"1 = %s", fg.getRelation(i).str().c_str()];
            }
        }
        self.fundRelsDetails.text = rels;
        self.fundRelsDetails.font = self.fundRels.font;
        self.fundRelsDetails.textContainerInset = UIEdgeInsetsMake(10, 10, 10, 10);
    }

    // TODO: TV
}

- (IBAction)tvCalculate:(id)sender {
}

@end
