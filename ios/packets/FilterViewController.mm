
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

#import "FilterViewController.h"
#import "NewPacketController.h"
#import "PacketManagerIOS.h"
#import "ReginaHelper.h"
#import "surfaces/surfacefilter.h"

static NSArray* orientabilityText;
static NSArray* compactnessText;
static NSArray* boundaryText;
static NSMutableCharacterSet* eulerSeparators;

#pragma mark - Property-based filter

@interface FilterPropertiesViewController () <UITextFieldDelegate> {
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UISegmentedControl *orientability;
@property (weak, nonatomic) IBOutlet UILabel *orientabilityExpln;
@property (weak, nonatomic) IBOutlet UISegmentedControl *compactness;
@property (weak, nonatomic) IBOutlet UILabel *compactnessExpln;
@property (weak, nonatomic) IBOutlet UISegmentedControl *boundary;
@property (weak, nonatomic) IBOutlet UILabel *boundaryExpln;
@property (weak, nonatomic) IBOutlet UITextField *euler;
@property (weak, nonatomic) IBOutlet UILabel *eulerExpln;
@property (assign, nonatomic) regina::SurfaceFilterProperties* packet;
@end

@implementation FilterPropertiesViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    if (! orientabilityText)
        [FilterPropertiesViewController initArrays];

    self.euler.delegate = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    if (myEdit)
        return;

    self.orientability.selectedSegmentIndex = [FilterPropertiesViewController selectionFromSet:self.packet->orientability()];
    self.compactness.selectedSegmentIndex = [FilterPropertiesViewController selectionFromSet:self.packet->compactness()];
    self.boundary.selectedSegmentIndex = [FilterPropertiesViewController selectionFromSet:self.packet->realBoundary()];

    [self updateEulerDisplay];

    // Update the description labels.
    // This will not trigger a change event on the packet, since the various
    // setFoo(...) routines only fire a change event if the new value differs from the old.
    [self orientabilityChanged:nil];
    [self compactnessChanged:nil];
    [self boundaryChanged:nil];
}

- (void)updateEulerDisplay
{
    const std::set<regina::LargeInteger>& ECs = self.packet->eulerChars();
    if (ECs.empty()) {
        self.euler.text = @"";
        self.eulerExpln.text = @"No restrictions on Euler characteristic.";
    } else if (ECs.size() == 1) {
        self.euler.text = [NSString stringWithUTF8String:(*ECs.begin()).stringValue().c_str()];
        self.eulerExpln.text = @"The surface must have this exact Euler characteristic.";
    } else {
        std::set<regina::LargeInteger>::reverse_iterator it = ECs.rbegin();
        NSMutableString* ans = [NSMutableString stringWithUTF8String:(*it).stringValue().c_str()];
        for (++it; it != ECs.rend(); ++it)
            [ans appendFormat:@", %s", (*it).stringValue().c_str()];
        self.euler.text = ans;
        self.eulerExpln.text = @"The surface must have one of these Euler characteristics.";
    }
}

- (IBAction)orientabilityChanged:(id)sender {
    myEdit = YES;
    long selection = self.orientability.selectedSegmentIndex;
    self.orientabilityExpln.text = orientabilityText[selection];
    self.packet->setOrientability([FilterPropertiesViewController setFromSelection:selection]);
    myEdit = NO;
}

- (IBAction)compactnessChanged:(id)sender {
    myEdit = YES;
    long selection = self.compactness.selectedSegmentIndex;
    self.compactnessExpln.text = compactnessText[selection];
    self.packet->setCompactness([FilterPropertiesViewController setFromSelection:selection]);
    myEdit = NO;
}

- (IBAction)boundaryChanged:(id)sender {
    myEdit = YES;
    long selection = self.boundary.selectedSegmentIndex;
    self.boundaryExpln.text = boundaryText[selection];
    self.packet->setRealBoundary([FilterPropertiesViewController setFromSelection:selection]);
    myEdit = NO;
}

+ (regina::BoolSet)setFromSelection:(long)selection
{
    switch (selection) {
        case 0: return regina::BoolSet::sBoth;
        case 1: return regina::BoolSet::sTrue;
        case 2: return regina::BoolSet::sFalse;
        default: return regina::BoolSet::sNone;
    }
}

+ (long)selectionFromSet:(const regina::BoolSet&)set
{
    if (set == regina::BoolSet::sBoth)
        return 0;
    if (set == regina::BoolSet::sTrue)
        return 1;
    if (set == regina::BoolSet::sFalse)
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

    std::set<regina::LargeInteger> set;
    bool valid;
    for (NSString* eulerStr in [textField.text componentsSeparatedByCharactersInSet:eulerSeparators]) {
        // Multiple spaces will result in empty strings.
        if (eulerStr.length == 0)
            continue;

        regina::LargeInteger euler([eulerStr UTF8String], 10, &valid);
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

    myEdit = YES;
    self.packet->setEulerChars(set);
    [self updateEulerDisplay];
    myEdit = NO;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self.euler resignFirstResponder];
    return NO;
}

@end

#pragma mark - Combination-based filter

@interface FilterCombinationViewController () <UITableViewDataSource, UITableViewDelegate, PacketDelegate> {
    NSPointerArray *_subfilters;
    PacketListenerIOS *_listener;
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UITableView *children;
@property (weak, nonatomic) IBOutlet UISegmentedControl *type;
@property (assign, nonatomic) regina::SurfaceFilterCombination* packet;
@end

@implementation FilterCombinationViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.children.dataSource = self;
    self.children.delegate = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    if (myEdit)
        return;

    self.type.selectedSegmentIndex = (self.packet->usesAnd() ? 0 : 1);

    [self updateSubfilters];

    // Listen on child renames / additions / deletions.
    if (! _listener)
        _listener = [PacketListenerIOS listenerWithPacket:self.packet delegate:self listenChildren:NO];
}

- (void)dealloc
{
    [_listener permanentlyUnlisten];
}

- (void)endEditing
{
    // Nothing to do: edits are committed as soon as they happen via typeChanged:.
}

- (void)updateSubfilters
{
    _subfilters = [NSPointerArray pointerArrayWithOptions:NSPointerFunctionsOpaqueMemory | NSPointerFunctionsOpaquePersonality];

    regina::Packet* p;
    for (p = self.packet->firstChild(); p; p = p->nextSibling())
        if (p->type() == regina::PACKET_SURFACEFILTER)
            [_subfilters addPointer:p];

    [self.children reloadData];
}

- (IBAction)typeChanged:(id)sender {
    myEdit = YES;
    self.packet->setUsesAnd(self.type.selectedSegmentIndex == 0);
    myEdit = NO;
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
        cell.textLabel.text = [NSString stringWithUTF8String:static_cast<regina::Packet*>([_subfilters pointerAtIndex:indexPath.row])->label().c_str()];
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

    [ReginaHelper viewPacket:static_cast<regina::Packet*>([_subfilters pointerAtIndex:indexPath.row])];
}

- (void)childWasAddedTo:(regina::Packet *)packet child:(regina::Packet *)child
{
    // Be brutal for now: just reload the entire table.
    [self updateSubfilters];
}

- (void)childWasRemovedFrom:(regina::Packet *)packet child:(regina::Packet *)child inParentDestructor:(bool)d
{
    // Be brutal for now: just reload the entire table.
    [self updateSubfilters];
}

- (void)childWasRenamed:(regina::Packet *)packet child:(regina::Packet *)child
{
    // Be brutal for now: just reload the entire table.
    [self updateSubfilters];
}

@end
