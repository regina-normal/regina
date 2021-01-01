
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#import "MBProgressHUD.h"
#import "LinkViewController.h"
#import "LinkCrossings.h"
#import "LinkMoves.h"
#import "ReginaHelper.h"
#import "engine.h"
#import "link/link.h"
#import "progress/progresstracker.h"
#import "triangulation/dim3.h"

#define KEY_LINK_CROSSINGS_STYLE @"LinkCrossingsStyle"

static UIColor* unknotColour = [UIColor colorWithRed:0.5
                                               green:0.5
                                                blue:0.5
                                               alpha:1.0];

static NSString* unknotText = @"Unknot, no crossings";

#pragma mark - Crossing cell

@interface LinkCrossingCell : UICollectionViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@end

@implementation LinkCrossingCell
@end

#pragma mark - Component header

@interface LinkComponentHeader : UICollectionReusableView
@property (weak, nonatomic) IBOutlet UILabel *label;
@end

@implementation LinkComponentHeader
@end

#pragma mark - Link crossings

@interface LinkCrossings () <UICollectionViewDataSource, UICollectionViewDelegateFlowLayout, UIActionSheetDelegate> {
    NSMutableArray* components;
    NSString* refLabel;
    CGSize sizeLabel;
    CGSize sizeHeader;
    regina::ProgressTrackerOpen* simplifyTracker; // for cancellation
}

@property (weak, nonatomic) IBOutlet UILabel *header;

@property (weak, nonatomic) IBOutlet UISegmentedControl *style;
@property (weak, nonatomic) IBOutlet UICollectionView *crossings;
@property (weak, nonatomic) IBOutlet UIButton *actionsButton;

@property (assign, nonatomic) regina::Link* packet;


@property (strong, nonatomic) NSLock* simplifyLock; // locks the simplifyTracker _pointer_, not the tracker itself.
@end

@implementation LinkCrossings

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.style.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LINK_CROSSINGS_STYLE];

    self.packet = static_cast<regina::Link*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    self.crossings.delegate = self;
    self.crossings.dataSource = self;

    components = [[NSMutableArray alloc] initWithCapacity:self.packet->countComponents()];
    [self reloadPacket];

    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.crossings addGestureRecognizer:r];
}

- (void)reloadPacket
{
    [static_cast<LinkViewController*>(self.parentViewController) updateHeader:self.header];

    // Cache the crossings for each component.
    [components removeAllObjects];
    for (size_t i = 0; i < self.packet->countComponents(); ++i) {
        NSMutableArray* comp = [[NSMutableArray alloc] init];
        regina::StrandRef start = self.packet->component(i);
        if (start.crossing()) {
            regina::StrandRef s = start;
            do {
                [comp addObject:[NSValue valueWithBytes:&s objCType:@encode(regina::StrandRef)]];
                ++s;
            } while (s != start);
        }
        [components addObject:comp];
    }

    if (self.packet->size() < 10)
        refLabel = @"0";
    else if (self.packet->size() < 20)
        refLabel = @"10";
    else if (self.packet->size() < 100)
        refLabel = @"00";
    else if (self.packet->size() < 200)
        refLabel = @"100";
    else if (self.packet->size() < 1000)
        refLabel = @"000";
    else if (self.packet->size() < 2000)
        refLabel = @"1000";
    else if (self.packet->size() < 10000)
        refLabel = @"0000";
    else
        refLabel = @"00000";

    if (self.style.selectedSegmentIndex == 0) {
        // Pictorial display
        sizeLabel = [refLabel sizeWithAttributes:@{NSFontAttributeName: self.header.font}];
    } else {
        // Text display
        sizeLabel = [[NSString stringWithFormat:@"%@₊", refLabel] sizeWithAttributes:@{NSFontAttributeName: self.header.font}];
    }
    sizeHeader = [refLabel sizeWithAttributes:@{NSFontAttributeName: [UIFont boldSystemFontOfSize:self.header.font.pointSize]}];

    [self.crossings reloadData];
}

- (IBAction)complement:(id)sender {
    regina::Triangulation<3>* c = self.packet->complement();
    c->setLabel(self.packet->adornedLabel("Complement"));

    self.packet->insertChildLast(c);
    [ReginaHelper viewPacket:c];
}

- (IBAction)simplifyCancel:(id)sender
{
    // Cancel the exhaustive simplification, if one is running.
    
    // Note: the lock is created before the HUD (and the HUD calls this routine),
    // so by the time this routine is called, we are guaranteed that
    // self.simplifyLock is non-nil.
    
    [self.simplifyLock lock];
    if (self->simplifyTracker)
        self->simplifyTracker->cancel();
    [self.simplifyLock unlock];
}

- (void)simplifyExhaustiveWithHeight:(int)height
{
    if (! self.simplifyLock)
        self.simplifyLock = [[NSLock alloc] init];
    
    // Run the exhaustive simplification within a HUD.
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    UIView* root = [UIApplication sharedApplication].keyWindow.rootViewController.view;
    MBProgressHUD* hud = [MBProgressHUD showHUDAddedTo:root animated:YES];
    hud.label.text = @"Searching Reidemeister graph…";
    
    [hud.button setTitle:@"Cancel" forState:UIControlStateNormal];
    [hud.button addTarget:self action:@selector(simplifyCancel:) forControlEvents:UIControlEventTouchUpInside];
    
    size_t initSize = self.packet->size();
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        regina::ProgressTrackerOpen* tracker = new regina::ProgressTrackerOpen();
        bool cancelled = false;
        
        bool knot = (self.packet->countComponents() == 1);
        
        [self.simplifyLock lock];
        self->simplifyTracker = tracker;
        [self.simplifyLock unlock];
        
        self.packet->simplifyExhaustive(height, regina::politeThreads(), tracker);
        
        unsigned long steps;
        while (! tracker->isFinished()) {
            if (tracker->stepsChanged()) {
                steps = tracker->steps();
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (knot)
                        hud.detailsLabel.text = [NSString stringWithFormat:@"Tried %ld knots", steps];
                    else
                        hud.detailsLabel.text = [NSString stringWithFormat:@"Tried %ld links", steps];
                });
            }
            usleep(100000);
        }
        [self.simplifyLock lock];
        self->simplifyTracker = nullptr;
        [self.simplifyLock unlock];
        
        cancelled = tracker->isCancelled();
        delete tracker;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [hud hideAnimated:NO];
            [UIApplication sharedApplication].idleTimerDisabled = NO;
            
            if ((! cancelled) && self.packet->size() == initSize) {
                // We still couldn't simplify.
                UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Could Not Simplify"
                                                                               message:[NSString stringWithFormat:@"I still could not simplify the %@, even after exhaustively searching the Reidemeister graph up to %ld crossings.\n\nI can look further, but be warned: the time and memory required could grow very rapidly.",
                                                                                        (knot ? @"knot" : @"link"),
                                                                                        initSize + height]
                                                                        preferredStyle:UIAlertControllerStyleAlert];
                [alert addAction:[UIAlertAction actionWithTitle:@"Keep trying"
                                                          style:UIAlertActionStyleDefault
                                                        handler:^(UIAlertAction* action) {
                                                            [self simplifyExhaustiveWithHeight:(height + 1)];
                                                        }]];
                [alert addAction:[UIAlertAction actionWithTitle:@"Close"
                                                          style:UIAlertActionStyleCancel
                                                        handler:^(UIAlertAction * action) {}]];
                [self presentViewController:alert animated:YES completion:nil];
            }
        });
    });
    
}

- (IBAction)simplify:(id)sender
{
    if (self.packet->isEmpty()) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"This Link is Empty"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    if (! self.packet->intelligentSimplify()) {
        // Greedy simplification failed.
        // Offer a more exhaustive alternative.
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Could Not Simplify"
                                                                       message:nil
                                                                preferredStyle:UIAlertControllerStyleAlert];
        if (self.packet->countComponents() == 1) {
            [alert addAction:[UIAlertAction actionWithTitle:@"Try harder"
                                                      style:UIAlertActionStyleDefault
                                                    handler:^(UIAlertAction* action) {
                                                        [self simplifyExhaustiveWithHeight:1];
                                                    }]];
        }
        [alert addAction:[UIAlertAction actionWithTitle:@"Close"
                                                  style:UIAlertActionStyleCancel
                                                handler:^(UIAlertAction * action) {}]];
        [self presentViewController:alert animated:YES completion:nil];
    }
}

- (IBAction)reflect:(id)sender {
    self.packet->reflect();
}

- (IBAction)rotate:(id)sender {
    self.packet->rotate();
}

- (IBAction)reverse:(id)sender {
    self.packet->reverse();
}

- (IBAction)reidemeister:(id)sender {
    // Note: LinkMoves lives on the same storyboard as LinkCrossings.
    UIViewController* sheet = [self.storyboard instantiateViewControllerWithIdentifier:@"linkMoves"];
    static_cast<LinkMoves*>(sheet).packet = self.packet;
    [self presentViewController:sheet animated:YES completion:nil];
}

- (IBAction)actions:(id)sender {
    UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                         destructiveButtonTitle:nil
                                              otherButtonTitles:@"Reflect",
                                                                @"Rotate",
                                                                @"Reverse",
                                                                @"Reidemeister moves",
                                                                nil];
    [sheet showFromRect:self.actionsButton.frame inView:self.view animated:YES];
}

- (IBAction)styleChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setInteger:self.style.selectedSegmentIndex forKey:KEY_LINK_CROSSINGS_STYLE];
    [self reloadPacket];
}

- (IBAction)longPress:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    UIGestureRecognizerState state = press.state;

    CGPoint location = [press locationInView:self.crossings];
    NSIndexPath *indexPath = [self.crossings indexPathForItemAtPoint:location];

    if (! indexPath)
        return;
    if (indexPath.section >= components.count)
        return;
    if (indexPath.row >= ((NSArray*)components[indexPath.section]).count)
        return;

    regina::StrandRef s;
    [(NSValue*)components[indexPath.section][indexPath.row] getValue:&s];

    if (state == UIGestureRecognizerStateBegan) {
        LinkCrossingCell *cell = static_cast<LinkCrossingCell*>([self.crossings cellForItemAtIndexPath:indexPath]);

        UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil
                                                                       message:nil
                                                                preferredStyle:UIAlertControllerStyleActionSheet];
        [alert addAction:[UIAlertAction actionWithTitle:[NSString stringWithFormat:@"Change crossing %d", s.crossing()->index()]
                                                  style:UIAlertActionStyleDefault
                                                handler:^(UIAlertAction*){
                                                    self.packet->change(s.crossing());
                                                }]];
        [alert addAction:[UIAlertAction actionWithTitle:[NSString stringWithFormat:@"Resolve crossing %d", s.crossing()->index()]
                                                  style:UIAlertActionStyleDefault
                                                handler:^(UIAlertAction*){
                                                    self.packet->resolve(s.crossing());
                                                }]];
        [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                  style:UIAlertActionStyleCancel
                                                handler:^(UIAlertAction*) {
                                                }]];
        [alert setModalPresentationStyle:UIModalPresentationPopover];
        alert.popoverPresentationController.sourceView = self.crossings;
        alert.popoverPresentationController.sourceRect = cell.frame;
        [self presentViewController:alert animated:YES completion:nil];
    }
}

#pragma mark - Collection view

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return self.packet->countComponents();
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    if (self.packet->component(section).crossing())
        return ((NSArray*)components[section]).count;
    else
        return 1; // zero-crossing component
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    if (! self.packet->component(indexPath.section).crossing()) {
        // This is a zero-crossing component.
        LinkCrossingCell* cell = (LinkCrossingCell*)[collectionView dequeueReusableCellWithReuseIdentifier:@"text" forIndexPath:indexPath];
        cell.index.text = unknotText;
        cell.index.textColor = unknotColour;
        return cell;
    }

    regina::StrandRef s;
    [(NSValue*)components[indexPath.section][indexPath.row] getValue:&s];

    if (self.style.selectedSegmentIndex == 0) {
        // Pictorial display.
        NSString* cellId;
        if (s.crossing()->sign() > 0) {
            if (s.strand() == 0)
                cellId = @"+l";
            else
                cellId = @"+u";
        } else {
            if (s.strand() == 0)
                cellId = @"-l";
            else
                cellId = @"-u";
        }

        LinkCrossingCell* cell = (LinkCrossingCell*)[collectionView dequeueReusableCellWithReuseIdentifier:cellId forIndexPath:indexPath];
        cell.index.text = [NSString stringWithFormat:@"%d", s.crossing()->index()];
        return cell;
    } else {
        // Text-based display.
        LinkCrossingCell* cell = (LinkCrossingCell*)[collectionView dequeueReusableCellWithReuseIdentifier:@"text" forIndexPath:indexPath];

        if (s.crossing()->sign() > 0) {
            if (s.strand() == 0)
                cell.index.text = [NSString stringWithFormat:@"%d₊", s.crossing()->index()];
            else
                cell.index.text = [NSString stringWithFormat:@"%d⁺", s.crossing()->index()];
            cell.index.textColor = LinkViewController.posColour;
        } else {
            if (s.strand() == 0)
                cell.index.text = [NSString stringWithFormat:@"%d₋", s.crossing()->index()];
            else
                cell.index.text = [NSString stringWithFormat:@"%d⁻", s.crossing()->index()];
            cell.index.textColor = LinkViewController.negColour;
        }

        return cell;
    }
}

- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath {
    LinkComponentHeader* header = (LinkComponentHeader*)[collectionView dequeueReusableSupplementaryViewOfKind:kind withReuseIdentifier:@"component" forIndexPath:indexPath];
    header.label.text = [NSString stringWithFormat:@"Component %ld", (long)indexPath.section];
    return header;
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    if (! self.packet->component(indexPath.section).crossing()) {
        // This is a zero-crossing component.
        CGSize text = [unknotText sizeWithAttributes:@{NSFontAttributeName: self.header.font}];
        return CGSizeMake(ceil(text.width + 10),
                          ceil(text.height) + 10);
    }

    if (self.style.selectedSegmentIndex == 0) {
        // Pictorial display

        // Labels are positioned 10 points from the left edge of the cell, though
        // the UILabel itself will add a bit more padding.
        // The crossing image in the cell is 22x22, and we need to clear a bit more than
        // halfway in both directions.
        // We give a minimum width of 40 because, if the original width is smaller,
        // it means there are very few crossings and we do not exactly need to cram
        // things in.
        return CGSizeMake(fmax(ceil(sizeLabel.width) + 18, 40),
                          ceil(sizeLabel.height) + 18);
    } else {
        // Text display
        // Labels are vertically centred, horizontally push to the very left of the cell.
        return CGSizeMake(ceil(sizeLabel.width) + 10,
                          ceil(sizeLabel.height) + 10);
    }
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section {
    return CGSizeMake(0, ceil(sizeHeader.height) + 30);
}

#pragma mark - Action sheet

- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    // Note: We implement didDismissWithButtonIndex: instead of clickedButtonAtIndex:,
    // since this ensures that the action sheet popover is already dismissed before we
    // try to open any other popover (such as the Reidemeister moves view).
    switch (buttonIndex) {
        case 0:
            [self reflect:nil]; break;
        case 1:
            [self rotate:nil]; break;
        case 2:
            [self reverse:nil]; break;
        case 3:
            [self reidemeister:nil]; break;
    }
}

@end
