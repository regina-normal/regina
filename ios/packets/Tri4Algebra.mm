
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

#import "ReginaHelper.h"
#import "TextHelper.h"
#import "Tri4ViewController.h"
#import "Tri4Algebra.h"
#import "triangulation/dim4.h"
#import "utilities/stringutils.h"

@interface Tri4Algebra ()
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UILabel *h1;
@property (weak, nonatomic) IBOutlet UILabel *h2;
@property (weak, nonatomic) IBOutlet UILabel *fundName;
@property (weak, nonatomic) IBOutlet UILabel *fundGens;
@property (weak, nonatomic) IBOutlet UILabel *fundRels;
@property (weak, nonatomic) IBOutlet UITextView *fundRelsDetails;

@property (strong, nonatomic) Tri4ViewController* viewer;
@property (assign, nonatomic) regina::Triangulation<4>* packet;
@end

@implementation Tri4Algebra

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Tri4ViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    [self reloadPacket];
}

+ (void)reloadGroup:(const regina::GroupPresentation &)group name:(UILabel *)name gens:(UILabel *)gens rels:(UILabel *)rels details:(UITextView *)details
{
    std::string groupName = group.recogniseGroup(true);
    if (groupName.length()) {
        name.text = [@(groupName.c_str()) stringByReplacingOccurrencesOfString:@"↦" withString:@"→"];
    } else {
        name.attributedText = [TextHelper dimString:@"Not recognised"];
    }
    
    size_t nGens = group.countGenerators();
    bool alphabetic = (nGens <= 26);
    if (nGens == 0)
        gens.text = @"No generators";
    else if (nGens == 1)
        gens.text = @"1 generator: a";
    else if (nGens == 2)
        gens.text = @"2 generators: a, b";
    else if (alphabetic)
        gens.text = [NSString stringWithFormat:@"%ld generators: a ... %c",
                     nGens, char('a' + nGens - 1)];
    else
        gens.text = [NSString stringWithFormat:@"%ld generators: g0 ... g%ld",
                     nGens, nGens - 1];
    
    size_t nRels = group.countRelations();
    if (nRels == 0) {
        rels.text = @"No relations";
    } else if (nRels == 1) {
        rels.text = @"1 relation:";
    } else {
        rels.text = [NSString stringWithFormat:@"%ld relations:", nRels];
    }
    
    NSMutableString* relsText = [[NSMutableString alloc] init];
    if (alphabetic) {
        // Generators are a, b, ...
        for (long i = 0; i < nRels; ++i) {
            NSMutableString* rel;
            const std::list<regina::GroupExpressionTerm>& terms(group.relation(i).terms());
            if (terms.empty())
                rel = [[NSMutableString alloc] initWithString:@"1"];
            else {
                rel = [[NSMutableString alloc] initWithString:@"1 ="];
                std::list<regina::GroupExpressionTerm>::const_iterator it;
                for (it = terms.begin(); it != terms.end(); ++it) {
                    [rel appendString:@" "];
                    if (it->exponent == 0)
                        [rel appendString:@"1"];
                    else {
                        [rel appendFormat:@"%c", char('a' + it->generator)];
                        if (it->exponent != 1)
                            [rel appendString:@(regina::superscript(it->exponent).c_str())];
                    }
                }
            }
            if (i > 0)
                [relsText appendString:@"\n"];
            [relsText appendString:rel];
        }
    } else {
        // Generators are g0, g1, ...
        // This is the default text that comes from the calculation engine.
        for (long i = 0; i < nRels; ++i) {
            if (i > 0)
                [relsText appendString:@"\n"];
            [relsText appendFormat:@"1 = %s", group.relation(i).str().c_str()];
        }
    }
    details.text = relsText;
    details.font = rels.font;
    details.textContainerInset = UIEdgeInsetsMake(10, 10, 10, 10);
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.header lockIcon:self.lockIcon];

    // Simplify before doing any algebraic computations.
    regina::Triangulation<4> t(*self.packet);
    t.intelligentSimplify();

    if (self.packet->isValid()) {
        self.h1.text = @(t.homologyH1().utf8().c_str());
        self.h2.text = @(t.homologyH2().utf8().c_str());
    } else {
        self.h1.text = self.h2.text = @"Invalid";
    }

    if (! self.packet->isValid()) {
        self.fundName.text = @"Invalid";
        self.fundGens.text = self.fundRels.text = self.fundRelsDetails.text = @"";
    } else if (t.countComponents() > 1) {
        self.fundName.text = @"Disconnected";
        self.fundGens.text = self.fundRels.text = self.fundRelsDetails.text = @"";
    } else {
        [Tri4Algebra reloadGroup:self.packet->fundamentalGroup()
                               name:self.fundName
                               gens:self.fundGens
                               rels:self.fundRels
                            details:self.fundRelsDetails];
    }
}

@end
