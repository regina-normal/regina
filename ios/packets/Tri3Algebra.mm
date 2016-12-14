
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
#import "TextPopover.h"
#import "Tri3ViewController.h"
#import "Tri3Algebra.h"
#import "MBProgressHUD.h"
#import "maths/numbertheory.h"
#import "progress/progresstracker.h"
#import "triangulation/dim3.h"
#import "utilities/stringutils.h"

// TODO: Turaev-Viro: reduce size of the label in the "Left Detail" table cells?

@interface TVItem : NSObject

@property (assign, nonatomic) unsigned long r;
@property (assign, nonatomic) bool parity;
@property (strong, nonatomic) NSString* value;

- (id)initWithValue:(NSString*)value r:(unsigned long)r parity:(bool)parity;
+ (id)itemWithValue:(NSString*)value r:(unsigned long)r parity:(bool)parity;

- (NSComparisonResult)compare:(TVItem*)rhs;

@end

@implementation TVItem

- (id)initWithValue:(NSString*)value r:(unsigned long)r parity:(bool)parity
{
    self = [super init];
    if (self) {
        _value = value;
        _r = r;
        _parity = parity;
    }
    return self;
}

+ (id)itemWithValue:(NSString*)value r:(unsigned long)r parity:(bool)parity
{
    return [[TVItem alloc] initWithValue:value r:r parity:parity];
}

- (NSComparisonResult)compare:(TVItem *)rhs
{
    if (self.r < rhs.r) return NSOrderedAscending;
    if (self.r > rhs.r) return NSOrderedDescending;
    if (self.r % 2) {
        if (self.parity && ! rhs.parity)
            return NSOrderedAscending;
        if (rhs.parity && ! self.parity)
            return NSOrderedDescending;
    }
    return NSOrderedSame;
}

@end

@interface Tri3Algebra () <UITableViewDataSource> {
    int r;
    NSMutableArray* computed;
    regina::ProgressTracker* tvTracker; // for cancellation
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

@property (strong, nonatomic) Tri3ViewController* viewer;
@property (assign, nonatomic) regina::Triangulation<3>* packet;

@property (strong, nonatomic) NSLock* tvLock; // locks the tvTracker _pointer_, not the tracker itself.
@end

@implementation Tri3Algebra

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Tri3ViewController*>(self.parentViewController);


    UILongPressGestureRecognizer *g = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(tvDetail:)];
    [self.tvValues addGestureRecognizer:g];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.tvValues.dataSource = self;

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
    
    unsigned long nGens = group.countGenerators();
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
    
    unsigned long nRels = group.countRelations();
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
    regina::Triangulation<3> t(*self.packet);
    t.intelligentSimplify();

    self.h1.text = @(t.homology().utf8().c_str());

    if (self.packet->isValid()) {
        self.h1Rel.text = @(t.homologyRel().utf8().c_str());
        self.h1Bdry.text = @(t.homologyBdry().utf8().c_str());
        self.h2.text = @(t.homologyH2().utf8().c_str());

        unsigned long coeffZ2 = t.homologyH2Z2();
        if (coeffZ2 == 0)
            self.h2z2.text = @"0";
        else if (coeffZ2 == 1)
            self.h2z2.text = @"ℤ₂";
        else
            self.h2z2.text = [NSString stringWithFormat:@"%ld ℤ₂", coeffZ2];
    } else {
        self.h1Rel.text = self.h1Bdry.text = self.h2.text = self.h2z2.text = @"Invalid";
    }

    if (t.countComponents() > 1) {
        self.fundName.text = @"Disconnected";
        self.fundGens.text = self.fundRels.text = self.fundRelsDetails.text = @"";
    } else {
        [Tri3Algebra reloadGroup:self.packet->fundamentalGroup()
                           name:self.fundName
                           gens:self.fundGens
                           rels:self.fundRels
                        details:self.fundRelsDetails];
    }

    computed = [[NSMutableArray alloc] init];
    const regina::Triangulation<3>::TuraevViroSet& s = self.packet->allCalculatedTuraevViro();
    for (const auto& tv : s)
        [computed addObject:[TVItem itemWithValue:@(tv.second.utf8("\u03B6" /* small zeta */).c_str())
                                                r:tv.first.first
                                           parity:tv.first.second]];

    [self.tvValues reloadData];
}

- (IBAction)tvCalculate:(id)sender {
    NSString* text = [self.tvArgs.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if (text.length == 0) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Missing Turaev-Viro Argument"
                                                        message:@"Please enter the argument r into the box provided."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    r = text.intValue;
    if (r == 0) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Turaev-Viro Argument"
                                                        message:@"The Turaev-Viro argument r should be a positive integer."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

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

    // Calculate the invariant!
    [self calculateTVWithParity:(r % 2)];
}

- (IBAction)tvCancel:(id)sender
{
    // Cancel the Turaev-Viro computation, if one is running.

    // Note: the lock is created before the HUD (and the HUD calls this routine),
    // so by the time this routine is called, we are guaranteed that self.tvLock
    // is non-nil.

    [self.tvLock lock];
    if (self->tvTracker)
        self->tvTracker->cancel();
    [self.tvLock unlock];
}

// This must only be called by calculateTV.
// For odd r, the parity=true version of this method calls the parity=false version.
- (void)calculateTVWithParity:(bool)parity
{
    const auto& s = self.packet->allCalculatedTuraevViro();
    if (s.find(std::make_pair(r, parity)) != s.end()) {
        // This has already been computed.
        // Move on to the next invariant (if any).
        if ((r % 2) && parity)
            [self calculateTVWithParity:false];
        return;
    }

    if (! self.tvLock)
        self.tvLock = [[NSLock alloc] init];

    // Run the computation within a HUD.
    [UIApplication sharedApplication].idleTimerDisabled = YES;

    UIView* root = [UIApplication sharedApplication].keyWindow.rootViewController.view;
    MBProgressHUD* hud = [MBProgressHUD showHUDAddedTo:root animated:YES];
    hud.label.text = @"Computing invariant…";
    hud.mode = MBProgressHUDModeDeterminateHorizontalBar;
    hud.progress = 0;

    [hud.button setTitle:@"Cancel" forState:UIControlStateNormal];
    [hud.button addTarget:self action:@selector(tvCancel:) forControlEvents:UIControlEventTouchUpInside];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        regina::ProgressTracker* tracker = new regina::ProgressTracker();
        bool cancelled = false;

        [self.tvLock lock];
        self->tvTracker = tracker;
        [self.tvLock unlock];

        self.packet->turaevViro(r, parity, regina::TV_DEFAULT, tracker);

        NSString* desc;
        float progress;
        while (! tracker->isFinished()) {
            if (tracker->descriptionChanged()) {
                desc = [NSString stringWithUTF8String:tracker->description().c_str()];
                progress = tracker->percent() / 100;
                dispatch_async(dispatch_get_main_queue(), ^{
                    hud.detailsLabel.text = desc;
                    hud.progress = progress;
                });
            } else if (tracker->percentChanged()) {
                progress = tracker->percent() / 100;
                dispatch_async(dispatch_get_main_queue(), ^{
                    hud.progress = progress;
                });
            }
            usleep(250000);
        }
        [self.tvLock lock];
        self->tvTracker = nullptr;
        [self.tvLock unlock];

        cancelled = tracker->isCancelled();
        delete tracker;

        dispatch_async(dispatch_get_main_queue(), ^{
            [hud hideAnimated:NO];
            [UIApplication sharedApplication].idleTimerDisabled = NO;

            if (! cancelled) {
                // Now the call to turaevViro() should be instantaneous.
                NSString* value = @(self.packet->turaevViro(r, parity).utf8("\u03B6" /* small zeta */).c_str());
                TVItem* item = [TVItem itemWithValue:value r:r parity:parity];

                NSUInteger index = [computed indexOfObject:item
                                             inSortedRange:NSMakeRange(0, computed.count)
                                                   options:NSBinarySearchingInsertionIndex
                                           usingComparator:^(TVItem* x, TVItem* y) { return [x compare:y]; }];
                [computed insertObject:item atIndex:index];
                
                [self.tvValues insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:index inSection:0]]
                                     withRowAnimation:UITableViewRowAnimationRight];

                // Go on and calculate the next invariant (if any).
                if ((r % 2) && parity)
                    [self calculateTVWithParity:false];
            }
        });
    });
}

- (IBAction)tvDetail:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    UIGestureRecognizerState state = press.state;

    CGPoint location = [press locationInView:self.tvValues];
    NSIndexPath *indexPath = [self.tvValues indexPathForRowAtPoint:location];

    if (! indexPath)
        return;

    if (state == UIGestureRecognizerStateBegan) {
        UITableViewCell *cell = [self.tvValues cellForRowAtIndexPath:indexPath];
        TVItem* item = computed[indexPath.row];

        NSMutableAttributedString* detail = [[NSMutableAttributedString alloc] init];

        [detail appendAttributedString:[TextHelper markedString:[NSString stringWithFormat:@"Turaev-Viro invariant: "]]];
        if (item.r % 2 == 0) {
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"r = %ld\n\n", item.r]]];
        } else if (item.parity) {
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"r = %ld, odd\n\n", item.r]]];
        } else {
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"r = %ld, even\n\n", item.r]]];
        }

        [detail appendAttributedString:[TextHelper markedString:[NSString stringWithFormat:@"Value: "]]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:item.value]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n\n"]];

        [detail appendAttributedString:[TextHelper markedString:[NSString stringWithFormat:@"Root: "]]];

        unsigned long primitiveRoot = ((item.r % 2 && ! item.parity) ? item.r : 2 * item.r);
        NSString* ending;

        if (primitiveRoot % 10 == 1 && primitiveRoot % 100 != 11)
            ending = @"st";
        else if (primitiveRoot % 10 == 2 && primitiveRoot % 100 != 12)
            ending = @"nd";
        else if (primitiveRoot % 10 == 3 && primitiveRoot % 100 != 13)
            ending = @"rd";
        else
            ending = @"th";

        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"\u03B6 is a primitive %ld%@ root of unity", primitiveRoot, ending]]];

        [detail addAttributes:@{NSFontAttributeName: cell.detailTextLabel.font} range:NSMakeRange(0, detail.length)];

        TextPopover* c = [self.storyboard instantiateViewControllerWithIdentifier:@"textPopover"];
        c.text = detail;
        UIPopoverController* detailPopover = [[UIPopoverController alloc] initWithContentViewController:c];
        [detailPopover presentPopoverFromRect:cell.frame
                                       inView:self.tvValues
                     permittedArrowDirections:UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown
                                     animated:YES];
    }
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
    cell.detailTextLabel.text = item.value;
    if (item.r % 2) {
        if (item.parity)
            cell.textLabel.text = [NSString stringWithFormat:@"%ld, odd", item.r];
        else
            cell.textLabel.text = [NSString stringWithFormat:@"%ld, even", item.r];
    } else
        cell.textLabel.text = [NSString stringWithFormat:@"%ld", item.r];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

@end
