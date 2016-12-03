
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
#import "EltMoves4.h"
#import "Tri4ViewController.h"
#import "Tri4Gluings.h"
#import "packet/container.h"
#import "triangulation/dim4.h"

#pragma mark - Table cell

@interface Tri4GluingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *facet4;
@property (weak, nonatomic) IBOutlet UILabel *facet3;
@property (weak, nonatomic) IBOutlet UILabel *facet2;
@property (weak, nonatomic) IBOutlet UILabel *facet1;
@property (weak, nonatomic) IBOutlet UILabel *facet0;
@end

@implementation Tri4GluingCell
@end

#pragma mark - Triangulation<4> gluings

@interface Tri4Gluings () <UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate, UIActionSheetDelegate> {
    CGFloat headerHeight;
    UILabel* editLabel;
    UITextField* editField;
    int editSimplex;
    int editFacet; // -1 for editing the description
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UITableView *pentachora;
@property (weak, nonatomic) IBOutlet UIButton *orientButton;
@property (weak, nonatomic) IBOutlet UIButton *orientIcon;
@property (weak, nonatomic) IBOutlet UIButton *actionsButton;

@property (strong, nonatomic) Tri4ViewController* viewer;
@property (assign, nonatomic) regina::Triangulation<4>* packet;
@end

@implementation Tri4Gluings

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Tri4ViewController*>(self.parentViewController);
    
    UITapGestureRecognizer *r = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touched:)];
    [self.pentachora addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.pentachora.delegate = self;
    self.pentachora.dataSource = self;
    
    [self reloadPacket];

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(keyboardDidShow:) name:UIKeyboardDidShowNotification object:nil];
    [nc addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc removeObserver:self name:UIKeyboardDidShowNotification object:nil];
    [nc removeObserver:self name:UIKeyboardWillHideNotification object:nil];
}

- (void)reloadPacket
{
    if (myEdit)
        return;
    
    [self.viewer updateHeader:self.header lockIcon:self.lockIcon];
    self.orientButton.enabled = self.orientIcon.enabled = (self.packet->isOrientable() && ! self.packet->isOriented());
    [self.pentachora reloadData];
}

+ (NSString*)destStringFromFacet:(int)srcFacet dest:(regina::Pentachoron<4>*)destSimp gluing:(const regina::Perm<5>&)gluing
{
    if (! destSimp)
        return @" "; // Use a space to ensure the label has enough height to pick up touches.
    else
        return [NSString stringWithFormat:@"%ld (%s)",
                destSimp->markedIndex(),
                (gluing * regina::Tetrahedron<4>::ordering(srcFacet)).trunc4().c_str()];
}

- (IBAction)newSimplex:(id)sender {
    if (! self.packet->isPacketEditable())
        return;
    
    myEdit = YES;
    self.packet->newSimplex();
    myEdit = NO;
    
    // Update the necessary elements of the UI.
    NSIndexPath* last = [NSIndexPath indexPathForRow:self.packet->size()
                                           inSection:0];
    NSIndexPath* add = [NSIndexPath indexPathForRow:self.packet->size()+1
                                          inSection:0];
    [self.pentachora insertRowsAtIndexPaths:@[last]
                          withRowAnimation:UITableViewRowAnimationAutomatic];
    [self.pentachora scrollToRowAtIndexPath:add
                          atScrollPosition:UITableViewScrollPositionBottom animated:YES];
    
    [self.viewer updateHeader:self.header lockIcon:self.lockIcon];
}

- (void)endEditing
{
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [editField resignFirstResponder];
}

- (void)editGluingForSimplex:(int)simplex cell:(Tri4GluingCell*)cell label:(UILabel*)label
{
    editLabel = label;
    editSimplex = simplex;
    editFacet = label.tag;
    
    editField = [[UITextField alloc] initWithFrame:label.frame];
    editField.backgroundColor = cell.backgroundColor;
    editField.borderStyle = UITextBorderStyleNone;
    editField.placeholder = (editFacet >= 0 ? @"Gluing" : @"Name");
    editField.clearButtonMode = UITextFieldViewModeAlways;
    editField.returnKeyType = UIReturnKeyDone;
    editField.autocorrectionType = UITextAutocorrectionTypeNo;
    if (editFacet >= 0) {
        regina::Pentachoron<4>* s = self.packet->simplex(editSimplex);
        editField.text = [[Tri4Gluings destStringFromFacet:editFacet
                                                         dest:s->adjacentSimplex(editFacet)
                                                       gluing:s->adjacentGluing(editFacet)]
                          stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        editField.autocapitalizationType = UITextAutocapitalizationTypeNone;
        editField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
        editField.textAlignment = NSTextAlignmentRight;
    } else {
        editField.text = @(self.packet->simplex(simplex)->description().c_str());
        editField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
        editField.keyboardType = UIKeyboardTypeDefault;
        editField.textAlignment = NSTextAlignmentLeft;
    }
    editField.delegate = self;
    
    [cell addSubview:editField];
    [editField becomeFirstResponder];
    [editField selectAll:nil];
}

- (IBAction)touched:(id)sender {
    // Finish and process any other edit that is currently in progress.
    if (editField) {
        [editField resignFirstResponder];
        editField = nil;
    }
    
    if (! self.packet->isPacketEditable())
        return;

    UITapGestureRecognizer *tap = static_cast<UITapGestureRecognizer*>(sender);
    if (tap.state != UIGestureRecognizerStateRecognized)
        return;
    
    CGPoint location = [tap locationInView:self.pentachora];
    NSIndexPath *indexPath = [self.pentachora indexPathForRowAtPoint:location];
    if (indexPath.row == 0 || indexPath.row > self.packet->size())
        return;
    
    Tri4GluingCell* cell = static_cast<Tri4GluingCell*>([self.pentachora cellForRowAtIndexPath:indexPath]);
    CGPoint inner = [self.pentachora convertPoint:location toView:cell];
    if (CGRectContainsPoint(cell.index.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.index];
    else if (CGRectContainsPoint(cell.facet0.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.facet0];
    else if (CGRectContainsPoint(cell.facet1.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.facet1];
    else if (CGRectContainsPoint(cell.facet2.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.facet2];
    else if (CGRectContainsPoint(cell.facet3.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.facet3];
    else if (CGRectContainsPoint(cell.facet4.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.facet4];
}

#pragma mark - Triangulation operations

- (BOOL)checkEditable
{
    if (! self.packet->isPacketEditable()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Cannot Change Gluings"
                                                        message:@"This triangulation has associated normal hypersurfaces, and so you cannot change its pentachoron gluings."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return NO;
    }

    return YES;
}

- (IBAction)simplify:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    if (! self.packet->intelligentSimplify()) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could Not Simplify"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
    }
}

- (IBAction)orient:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;
    
    if (self.packet->isOriented()) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Already Oriented"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    bool hasOr = false;
    for (auto c : self.packet->components())
        if (c->isOrientable()) {
            hasOr = true;
            break;
        }
    if (! hasOr) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Orientable Components"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    self.packet->orient();
}

- (IBAction)extractComponents:(id)sender
{
    [self endEditing];
    
    if (self.packet->isEmpty()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Empty Triangulation"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    if (self.packet->isConnected()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Triangulation is Connected"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    // Where to insert the components?
    // If there are already children of this triangulation, insert
    // the new triangulations at a deeper level.
    regina::Packet* base;
    if (self.packet->firstChild()) {
        base = new regina::Container();
        self.packet->insertChildLast(base);
        base->setLabel(self.packet->adornedLabel("Components"));
    } else
        base = self.packet;

    // Make the split.
    size_t nComps = self.packet->splitIntoComponents(base);

    // Tell the user what happened.
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithFormat:@"%ld Components Extracted", nComps]
                                                    message:@"I have constructed a new triangulation for each component."
                                                   delegate:nil
                                          cancelButtonTitle:@"Close"
                                          otherButtonTitles:nil];
    [alert show];

    [ReginaHelper viewChildren:base];
}

- (IBAction)barycentricSubdivision:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    self.packet->barycentricSubdivision();
}

- (IBAction)idealToFinite:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    // Make sure there are in fact vertices to truncate.
    if (! self.packet->isIdeal()) {
        bool hasInvalidVertices = false;
        if (! self.packet->isValid())
            for (auto v : self.packet->vertices())
                if (! v->isValid()) {
                    hasInvalidVertices = true;
                    break;
                }
        if (! hasInvalidVertices) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Ideal Vertices"
                                                            message:@"This operation truncates ideal (or invalid) vertices to produce real boundary components."
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            return;
        }
    }
    
    regina::Packet::ChangeEventSpan span(self.packet);
    self.packet->idealToFinite();
    self.packet->intelligentSimplify();
}

- (IBAction)makeIdeal:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    if (! self.packet->hasBoundaryFacets()) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Boundary Tetrahedra"
                                                        message:@"This operation converts real boundary components, formed from boundary tetrahedra, into ideal vertices."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    regina::Packet::ChangeEventSpan span(self.packet);
    self.packet->finiteToIdeal();
    self.packet->intelligentSimplify();
}

- (IBAction)doubleCover:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    self.packet->makeDoubleCover();
}

- (IBAction)elementaryMoves:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    UIViewController* sheet = [self.storyboard instantiateViewControllerWithIdentifier:@"eltMoves4"];
    static_cast<EltMoves4*>(sheet).packet = self.packet;
    [self presentViewController:sheet animated:YES completion:nil];
}

- (IBAction)actions:(id)sender {
    UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                         destructiveButtonTitle:nil
                                              otherButtonTitles:@"Extract components",
                                                                @"Barycentric subdivision",
                                                                @"Truncate ideal vertices",
                                                                @"Make ideal",
                                                                @"Double cover",
                                                                @"Elementary moves",
                                                                nil];
    [sheet showFromRect:self.actionsButton.frame inView:self.view animated:YES];
}

#pragma mark - Keyboard notifications

- (void)keyboardDidShow:(NSNotification*)notification
{
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;

    CGRect tableInDetail = [self.parentViewController.view convertRect:self.pentachora.frame fromView:self.view];
    CGFloat unused = self.parentViewController.view.bounds.size.height - tableInDetail.origin.y - tableInDetail.size.height;

    if (kbSize.height <= unused)
        return;

    self.pentachora.contentInset = UIEdgeInsetsMake(0, 0, kbSize.height - unused, 0);
    self.pentachora.scrollIndicatorInsets = UIEdgeInsetsMake(0, 0, kbSize.height - unused, 0);

    [self.pentachora scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:editSimplex+1 inSection:0]
                           atScrollPosition:UITableViewScrollPositionNone
                                   animated:YES];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    self.pentachora.contentInset = UIEdgeInsetsZero;
    self.pentachora.scrollIndicatorInsets = UIEdgeInsetsZero;
}

#pragma mark - Text field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    if (editField != textField) {
        NSLog(@"Error: Mismatched text field when editing gluings.");
        return;
    }
    
    if (! self.packet->isPacketEditable()) {
        [editField removeFromSuperview];
        editField = nil;
        editLabel = nil;
        return;
    }
    
    regina::Pentachoron<4>* s = self.packet->simplex(editSimplex);
    
    NSMutableArray* toReload = [[NSMutableArray alloc] init];
    if (editFacet >= 0) {
        NSString* dest = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        if (dest.length == 0) {
            // We are making this a boundary facet.
            if (s->adjacentSimplex(editFacet)) {
                myEdit = YES;
                [toReload addObject:[NSIndexPath indexPathForRow:s->adjacentSimplex(editFacet)->markedIndex()+1 inSection:0]];
                s->unjoin(editFacet);
                editLabel.text = @" ";
                [self.viewer updateHeader:self.header lockIcon:self.lockIcon];
                myEdit = NO;
            }
        } else {
            NSRegularExpression* regex = [[NSRegularExpression alloc] initWithPattern:@"\\A(\\d+)[ ,\\(]+([0-4][0-4][0-4][0-4])[ \\)]*\\Z" options:0 error:nil];
            NSTextCheckingResult* result = [regex firstMatchInString:dest options:0 range:NSMakeRange(0, dest.length)];
            if (! result) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Gluing"
                                                                message:@"Please enter the gluing in the form pentachoron (facet).  For example, you could enter \"7 (2140)\", or just \"7 2140\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            int destSimplex = [[dest substringWithRange:[result rangeAtIndex:1]] intValue];
            if (destSimplex < 0 || destSimplex >= self.packet->size()) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Pentachoron"
                                                                message:@"Please enter the gluing in the form pentachoron (facet).  For example, you could enter \"7 (2140)\", or just \"7 2140\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            // We know at this point that we pass the regex, which means the adjacent facet
            // is in the form [0-4][0-4][0-4][0-4].
            int adjGluingAsInt = [dest substringWithRange:[result rangeAtIndex:2]].intValue;
            int adj0 = (adjGluingAsInt / 1000);
            int adj1 = (adjGluingAsInt % 1000) / 100;
            int adj2 = (adjGluingAsInt % 100) / 10;
            int adj3 = (adjGluingAsInt % 10);
            if (adj0 == adj1 || adj0 == adj2 || adj1 == adj2 || adj0 == adj3 || adj1 == adj3 || adj2 == adj3) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Facet"
                                                                message:@"Please enter the gluing in the form pentachoron (facet).  For example, you could enter \"7 (2140)\", or just \"7 2140\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            regina::Perm<5> destGluing = regina::Perm<5>(adj0, adj1, adj2, adj3, (10 - adj0 - adj1 - adj2 - adj3)) *
                regina::Tetrahedron<4>::ordering(editFacet).inverse();
            
            // Are we gluing the facet to itself?
            if (destSimplex == editSimplex && destGluing[editFacet] == editFacet) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cannot Glue a Facet to Itself"
                                                                message:nil
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            if (s->adjacentSimplex(editFacet) != self.packet->simplex(destSimplex) || destGluing != s->adjacentGluing(editFacet)) {
                // We are making a change, and it's a valid one.
                // Do it.
                myEdit = YES;
                {
                    regina::Packet::ChangeEventSpan span(self.packet);
                    
                    // First unglue from the old partner if it exists.
                    if (s->adjacentSimplex(editFacet)) {
                        [toReload addObject:[NSIndexPath indexPathForRow:s->adjacentSimplex(editFacet)->markedIndex()+1 inSection:0]];
                        s->unjoin(editFacet);
                    }
                    
                    // We are gluing the facet to a new partner.
                    int destFacet = destGluing[editFacet];
                    
                    // Does this new partner already have its own partner?
                    // If so, better unglue it.
                    regina::Pentachoron<4>* adj = self.packet->simplex(destSimplex);
                    if (adj->adjacentSimplex(destFacet)) {
                        NSIndexPath* path = [NSIndexPath indexPathForRow:adj->adjacentSimplex(destFacet)->markedIndex()+1 inSection:0];
                        if ([toReload indexOfObject:path] == NSNotFound)
                            [toReload addObject:path];
                        adj->unjoin(destFacet);
                    }
                    
                    // Glue the two facets together.
                    s->join(editFacet, adj, destGluing);
                    NSIndexPath* path = [NSIndexPath indexPathForRow:destSimplex+1 inSection:0];
                    if ([toReload indexOfObject:path] == NSNotFound)
                        [toReload addObject:path];
                    
                    editLabel.text = [NSString stringWithFormat:@"%d (%d%d%d%d)", destSimplex, adj0, adj1, adj2, adj3];
                }
                [self.viewer updateHeader:self.header lockIcon:self.lockIcon];
                myEdit = NO;
            }
        }
    } else {
        myEdit = YES;
        NSString* desc = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        s->setDescription(desc.UTF8String);
        editLabel.text = [NSString stringWithFormat:@"%d. %@", editSimplex, desc];
        myEdit = NO;
    }
    
cleanUpGluing:
    [editField removeFromSuperview];
    editField = nil;
    editLabel = nil;
    
    if (toReload.count)
        [self.pentachora reloadRowsAtIndexPaths:toReload withRowAnimation:UITableViewRowAnimationFade];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (self.packet->isPacketEditable() ? 2 : 1) + self.packet->size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    else if (indexPath.row == self.packet->size() + 1)
        return [tableView dequeueReusableCellWithIdentifier:@"Add"];
    
    Tri4GluingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Pentachoron" forIndexPath:indexPath];
    regina::Pentachoron<4>* s = self.packet->simplex(indexPath.row - 1);
    cell.index.text = [NSString stringWithFormat:@"%zd. %s", indexPath.row - 1, s->description().c_str()];
    cell.facet0.text = [Tri4Gluings destStringFromFacet:0 dest:s->adjacentSimplex(0) gluing:s->adjacentGluing(0)];
    cell.facet1.text = [Tri4Gluings destStringFromFacet:1 dest:s->adjacentSimplex(1) gluing:s->adjacentGluing(1)];
    cell.facet2.text = [Tri4Gluings destStringFromFacet:2 dest:s->adjacentSimplex(2) gluing:s->adjacentGluing(2)];
    cell.facet3.text = [Tri4Gluings destStringFromFacet:3 dest:s->adjacentSimplex(3) gluing:s->adjacentGluing(3)];
    cell.facet4.text = [Tri4Gluings destStringFromFacet:4 dest:s->adjacentSimplex(4) gluing:s->adjacentGluing(4)];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (self.packet->isPacketEditable())
        return (indexPath.row > 0 && indexPath.row <= self.packet->size());
    else
        return NO;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0 || indexPath.row > self.packet->size())
        return;
    
    // Many rows could change - not only do we blank out gluings for adjacent pentachora,
    // but we also need to reindex every pentachoron after the one that was removed.
    // Just reload everything.
    // This is easy: we don't set myEdit=YES, and instead rely on the automatic packet reload.
    self.packet->removeSimplexAt(indexPath.row - 1);
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.pentachora.rowHeight;
    
    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [self.pentachora dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

#pragma mark - Action sheet

- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    // Note: We implement didDismissWithButtonIndex: instead of clickedButtonAtIndex:,
    // since this ensures that the action sheet popover is already dismissed before we
    // try to open any other popover (such as the elementary moves view).
    switch (buttonIndex) {
        case 0:
            [self extractComponents:nil]; break;
        case 1:
            [self barycentricSubdivision:nil]; break;
        case 2:
            [self idealToFinite:nil]; break;
        case 3:
            [self makeIdeal:nil]; break;
        case 4:
            [self doubleCover:nil]; break;
        case 5:
            [self elementaryMoves:nil]; break;
    }
}

@end

