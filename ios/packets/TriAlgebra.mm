
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

#import "ReginaHelper.h"
#import "TextHelper.h"
#import "TriangulationViewController.h"
#import "TriAlgebra.h"
#import "maths/numbertheory.h"
#import "triangulation/ntriangulation.h"

#define TV_WARN_LARGE_R 15

@interface TVItem : NSObject

@property (assign, nonatomic) int r;
@property (assign, nonatomic) int root;
@property (assign, nonatomic) double value;

- (id)initWithValue:(double)value r:(int)r root:(int)root;
+ (id)itemWithValue:(double)value r:(int)r root:(int)root;

- (NSComparisonResult)compare:(TVItem*)rhs;

@end

@implementation TVItem

- (id)initWithValue:(double)value r:(int)r root:(int)root
{
    self = [super init];
    if (self) {
        _value = value;
        _r = r;
        _root = root;
    }
    return self;
}

+ (id)itemWithValue:(double)value r:(int)r root:(int)root
{
    return [[TVItem alloc] initWithValue:value r:r root:root];
}

- (NSComparisonResult)compare:(TVItem *)rhs
{
    if (self.r < rhs.r) return NSOrderedAscending;
    if (self.r > rhs.r) return NSOrderedDescending;
    if (self.root < rhs.root) return NSOrderedAscending;
    if (self.root > rhs.root) return NSOrderedDescending;
    return NSOrderedSame;
}

@end

@interface TriAlgebra () <UITableViewDataSource, UIAlertViewDelegate> {
    int r, root;
    NSMutableArray* computed;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

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

    self.tvValues.dataSource = self;

    [self reloadPacket];
}

+ (void)reloadGroup:(const regina::NGroupPresentation &)group name:(UILabel *)name gens:(UILabel *)gens rels:(UILabel *)rels details:(UITextView *)details
{
    std::string groupName = group.recogniseGroup();
    if (groupName.length()) {
        name.text = [@(groupName.c_str()) stringByReplacingOccurrencesOfString:@"↦" withString:@"→"];
    } else {
        name.attributedText = [TextHelper dimString:@"Not recognised"];
    }
    
    unsigned long nGens = group.getNumberOfGenerators();
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
    
    unsigned long nRels = group.getNumberOfRelations();
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
            const std::list<regina::NGroupExpressionTerm>& terms(group.getRelation(i).getTerms());
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
                [relsText appendString:@"\n"];
            [relsText appendString:rel];
        }
    } else {
        // Generators are g0, g1, ...
        // This is the default text that comes from the calculation engine.
        for (long i = 0; i < nRels; ++i) {
            if (i > 0)
                [relsText appendString:@"\n"];
            [relsText appendFormat:@"1 = %s", group.getRelation(i).str().c_str()];
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
        [TriAlgebra reloadGroup:self.packet->getFundamentalGroup()
                           name:self.fundName
                           gens:self.fundGens
                           rels:self.fundRels
                        details:self.fundRelsDetails];
    }

    computed = [[NSMutableArray alloc] init];
    const regina::NTriangulation::TuraevViroSet& s = self.packet->allCalculatedTuraevViro();
    for (regina::NTriangulation::TuraevViroSet::const_iterator it = s.begin(); it != s.end(); ++it)
        [computed addObject:[TVItem itemWithValue:it->second.evaluate(it->first.second).real() r:it->first.first root:it->first.second]];

    [self.tvValues reloadData];
}

- (IBAction)tvCalculate:(id)sender {
    NSRegularExpression* regex = [[NSRegularExpression alloc] initWithPattern:@"\\A[ \\(]*(\\d+)[ ,]+(\\d+)[ \\)]*\\Z" options:0 error:nil];
    NSTextCheckingResult* result = [regex firstMatchInString:self.tvArgs.text options:0 range:NSMakeRange(0, self.tvArgs.text.length)];
    if (! result) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Turaev-Viro Parameters"
                                                        message:@"Please enter the parameters (r, root) in the box provided.  "
                                                                 "These must be positive integers with 0 < root < 2r, and where "
                                                                 "root describes a (2r)th root of unity.  An example is (5, 3)."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    r = [[self.tvArgs.text substringWithRange:[result rangeAtIndex:1]] intValue];
    root = [[self.tvArgs.text substringWithRange:[result rangeAtIndex:2]] intValue];

    // Run sanity checks.
    if (! (self.packet->isValid() && self.packet->isClosed() && ! self.packet->isEmpty())) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cannot Calculate"
                                                        message:@"Turaev-Viro invariants are currently only available for closed, valid, non-empty triangulations."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    if (r < 3) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Requires r ≥ 3"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    if (root <= 0 || root >= 2 * r) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Requires 0 < root < 2r"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    if (regina::gcd(r, root) > 1) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Requires gcd(r, root) = 1"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    if (r >= TV_WARN_LARGE_R) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Long Calculation Ahead"
                                                        message:@"Turaev-Viro invariants require exponential time to compute, and you have chosen a large value of r.  "
                                                                 "Are you sure you wish to proceed?"
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                              otherButtonTitles:@"Compute", nil];
        [alert show];
        return;
    }

    // Calculate the invariant!
    [self calculateTV];
}

// This may be called from either the main queue or a background thread.
- (void)calculateTV
{
    const regina::NTriangulation::TuraevViroSet& s = self.packet->allCalculatedTuraevViro();
    if (s.find(std::make_pair(r, root)) != s.end()) {
        // Duplicate.
        return;
    }

    double value = self.packet->turaevViro(r, root).evaluate(root).real();
    TVItem* item = [TVItem itemWithValue:value r:r root:root];

    NSUInteger index = [computed indexOfObject:item
                                 inSortedRange:NSMakeRange(0, computed.count)
                                       options:NSBinarySearchingInsertionIndex
                               usingComparator:^(TVItem* x, TVItem* y) { return [x compare:y]; }];
    [computed insertObject:item atIndex:index];

    // The table needs to be updated in the main queue.
    dispatch_async(dispatch_get_main_queue(), ^{
        NSIndexPath* path = [NSIndexPath indexPathForRow:index inSection:0];
        [self.tvValues insertRowsAtIndexPaths:@[path] withRowAnimation:YES];
    });
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return computed.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    TVItem* item = computed[indexPath.row];
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"Value" forIndexPath:indexPath];
    cell.detailTextLabel.text = [NSString stringWithFormat:@"%lf", item.value];
    cell.textLabel.text = [NSString stringWithFormat:@"%d, %d:", item.r, item.root];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

#pragma mark - Alert view

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == alertView.cancelButtonIndex)
        return;

    [ReginaHelper runWithHUD:@"Calculating…"
                        code:^{
                            [self calculateTV];
                        }
                     cleanup:nil];
}

@end
