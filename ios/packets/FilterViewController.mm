
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

#import "FilterViewController.h"
#import "NewPacketController.h"
#import "PacketManagerIOS.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "surfaces/sfcombination.h"
#import "surfaces/sfproperties.h"

static NSArray* orientabilityText;
static NSArray* compactnessText;
static NSArray* boundaryText;
static NSMutableCharacterSet* eulerSeparators;

// TODO: Restore packet listener.

// TODO: Do we need to fix the "ambiguous row height" warning under iOS 8?

#pragma mark - Property-based filter

@interface FilterPropertiesViewController () <UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UISegmentedControl *orientability;
@property (weak, nonatomic) IBOutlet UILabel *orientabilityExpln;
@property (weak, nonatomic) IBOutlet UISegmentedControl *compactness;
@property (weak, nonatomic) IBOutlet UILabel *compactnessExpln;
@property (weak, nonatomic) IBOutlet UISegmentedControl *boundary;
@property (weak, nonatomic) IBOutlet UILabel *boundaryExpln;
@property (weak, nonatomic) IBOutlet UITextField *euler;
@property (weak, nonatomic) IBOutlet UILabel *eulerExpln;
@property (assign, nonatomic) regina::NSurfaceFilterProperties* packet;
@end

@implementation FilterPropertiesViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    if (! orientabilityText)
        [FilterPropertiesViewController initArrays];

    self.orientability.selectedSegmentIndex = [FilterPropertiesViewController selectionFromSet:self.packet->getOrientability()];
    self.compactness.selectedSegmentIndex = [FilterPropertiesViewController selectionFromSet:self.packet->getCompactness()];
    self.boundary.selectedSegmentIndex = [FilterPropertiesViewController selectionFromSet:self.packet->getRealBoundary()];

    [self updateEulerDisplay];
    self.euler.delegate = self;

    // Update the description labels.
    // This will not trigger a change event on the packet, since the various
    // setFoo(...) routines only fire a change event if the new value differs from the old.
    [self orientabilityChanged:nil];
    [self compactnessChanged:nil];
    [self boundaryChanged:nil];
}

- (void)updateEulerDisplay
{
    const std::set<regina::NLargeInteger>& ECs = self.packet->getECs();
    if (ECs.empty()) {
        self.euler.text = @"";
        self.eulerExpln.text = @"No restrictions on Euler characteristic.";
    } else if (ECs.size() == 1) {
        self.euler.text = [NSString stringWithUTF8String:(*ECs.begin()).stringValue().c_str()];
        self.eulerExpln.text = @"The surface must have this exact Euler characteristic.";
    } else {
        std::set<regina::NLargeInteger>::reverse_iterator it = ECs.rbegin();
        NSMutableString* ans = [NSMutableString stringWithUTF8String:(*it).stringValue().c_str()];
        for (++it; it != ECs.rend(); ++it)
            [ans appendFormat:@", %s", (*it).stringValue().c_str()];
        self.euler.text = ans;
        self.eulerExpln.text = @"The surface must have one of these Euler characteristics.";
    }
}

- (IBAction)orientabilityChanged:(id)sender {
    long selection = self.orientability.selectedSegmentIndex;
    self.orientabilityExpln.text = orientabilityText[selection];
    self.packet->setOrientability([FilterPropertiesViewController setFromSelection:selection]);
}

- (IBAction)compactnessChanged:(id)sender {
    long selection = self.compactness.selectedSegmentIndex;
    self.compactnessExpln.text = compactnessText[selection];
    self.packet->setCompactness([FilterPropertiesViewController setFromSelection:selection]);
}

- (IBAction)boundaryChanged:(id)sender {
    long selection = self.boundary.selectedSegmentIndex;
    self.boundaryExpln.text = boundaryText[selection];
    self.packet->setRealBoundary([FilterPropertiesViewController setFromSelection:selection]);
}

+ (regina::NBoolSet)setFromSelection:(long)selection
{
    switch (selection) {
        case 0: return regina::NBoolSet::sBoth;
        case 1: return regina::NBoolSet::sTrue;
        case 2: return regina::NBoolSet::sFalse;
        default: return regina::NBoolSet::sNone;
    }
}

+ (long)selectionFromSet:(const regina::NBoolSet&)set
{
    if (set == regina::NBoolSet::sBoth)
        return 0;
    if (set == regina::NBoolSet::sTrue)
        return 1;
    if (set == regina::NBoolSet::sFalse)
        return 2;
    // Hmm?
    NSLog(@"Filter-by-properties: some property was set to sNone, changing to sBoth.");
    return 0;
}

+ (void)initArrays
{
    orientabilityText = @[@" ",
                          @"The surface must be orientable.",
                          @"The surface must be non-orientable."];
    compactnessText = @[@" ",
                        @"The surface must be compact (not spun-normal).",
                        @"The surface must be spun-normal (i.e., have infinitely many triangles)."];
    boundaryText = @[@" ",
                     @"The surface must have boundary edges.",
                     @"The surface cannot have boundary edges."];
}

- (void)endEditing {
    // As a consequence, this calls eulerEditEnded, which is where the real work happens.
    [self.euler resignFirstResponder];
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    // Extract the set of Euler characteristics.
    if (eulerSeparators == nil) {
        eulerSeparators = [NSMutableCharacterSet whitespaceAndNewlineCharacterSet];
        [eulerSeparators addCharactersInString:@",()"];
    }

    std::set<regina::NLargeInteger> set;
    bool valid;
    for (NSString* eulerStr in [textField.text componentsSeparatedByCharactersInSet:eulerSeparators]) {
        // Multiple spaces will result in empty strings.
        if (eulerStr.length == 0)
            continue;

        regina::NLargeInteger euler([eulerStr UTF8String], 10, &valid);
        if (! valid) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Euler Characteristics Must Be Integers"
                                                            message:@"Please enter a list of allowed Euler characteristics, separated by commas or spaces."
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];

            // Reread the list from what is stored in the packet.
            [self updateEulerDisplay];
            return;
        }
        set.insert(euler);
    }

    self.packet->setECs(set);
    [self updateEulerDisplay];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self.euler resignFirstResponder];
    return NO;
}

@end

#pragma mark - Combination-based filter

@interface FilterCombinationViewController () <UITableViewDataSource, UITableViewDelegate> {
    NSPointerArray *_subfilters;
}
@property (weak, nonatomic) IBOutlet UITableView *children;
@property (weak, nonatomic) IBOutlet UISegmentedControl *type;
@property (assign, nonatomic) regina::NSurfaceFilterCombination* packet;
@end

@implementation FilterCombinationViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.type.selectedSegmentIndex = (self.packet->getUsesAnd() ? 0 : 1);

    self.children.dataSource = self;
    self.children.delegate = self;
    [self updateSubfilters];
    // TODO: Listen on child renames / additions / deletions.
}

- (void)endEditing
{
    // Nothing to do: edits are committed as soon as they happen via typeChanged:.
}

- (void)updateSubfilters
{
    _subfilters = [NSPointerArray pointerArrayWithOptions:NSPointerFunctionsOpaqueMemory | NSPointerFunctionsOpaquePersonality];

    regina::NPacket* p;
    for (p = self.packet->getFirstTreeChild(); p; p = p->getNextTreeSibling())
        if (p->getPacketType() == regina::PACKET_SURFACEFILTER)
            [_subfilters addPointer:p];

    [self.children reloadData];
}

- (IBAction)typeChanged:(id)sender {
    self.packet->setUsesAnd(self.type.selectedSegmentIndex == 0);
}

- (IBAction)addSubfilter:(id)sender {
    // Create a new child packet, but leave the master and detail displays as they are.
    NewPacketSpec* spec = [NewPacketSpec specWithType:regina::PACKET_SURFACEFILTER parent:self.packet];
    spec.viewOnCreation = NO;
    [PacketManagerIOS newPacket:spec];
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == _subfilters.count)
        return [self.children dequeueReusableCellWithIdentifier:@"Add" forIndexPath:indexPath];
    else {
        UITableViewCell* cell = [self.children dequeueReusableCellWithIdentifier:@"Subfilter" forIndexPath:indexPath];
        cell.textLabel.text = [NSString stringWithUTF8String:static_cast<regina::NPacket*>([_subfilters pointerAtIndex:indexPath.row])->getPacketLabel().c_str()];
        return cell;
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _subfilters.count + 1;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == _subfilters.count) {
        // This is the new subpacket cell.
        // Leave this work to the button on the cell.
        return;
    }

    PacketTreeController* tree = [ReginaHelper tree];
    [tree navigateToPacket:self.packet];

    // Hmm.  Selection does not work because the subtree is not yet
    // present in the master navigation controller (the animated transition
    // is still taking place at this point).
    // This is minor enough to leave as is for now.
    [ReginaHelper viewPacket:static_cast<regina::NPacket*>([_subfilters pointerAtIndex:indexPath.row])];
}

- (void)childWasAddedTo:(regina::NPacket *)packet child:(regina::NPacket *)child
{
    // Be brutal for now: just reload the entire table.
    [self updateSubfilters];
}

- (void)childWasRemovedFrom:(regina::NPacket *)packet child:(regina::NPacket *)child inParentDestructor:(bool)d
{
    // Be brutal for now: just reload the entire table.
    [self updateSubfilters];
}

- (void)childWasRenamed:(regina::NPacket *)packet child:(regina::NPacket *)child
{
    // Be brutal for now: just reload the entire table.
    [self updateSubfilters];
}

@end
