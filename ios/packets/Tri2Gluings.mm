
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
#import "Tri2Gluings.h"
#import "Tri2ViewController.h"
#import "packet/container.h"
#import "triangulation/dim2.h"

#pragma mark - Table cell

@interface Tri2GluingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *edge2;
@property (weak, nonatomic) IBOutlet UILabel *edge1;
@property (weak, nonatomic) IBOutlet UILabel *edge0;
@end

@implementation Tri2GluingCell
@end

#pragma mark - Triangulation<2> gluings viewer

@interface Tri2Gluings () <UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate, UIActionSheetDelegate> {
    CGFloat headerHeight;
    UILabel* editLabel;
    UITextField* editField;
    int editSimplex;
    int editEdge; // -1 for editing the description
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UILabel *properties;
@property (weak, nonatomic) IBOutlet UITableView *triangles;
@property (weak, nonatomic) IBOutlet UIButton *orientButton;
@property (weak, nonatomic) IBOutlet UIButton *orientIcon;
@property (weak, nonatomic) IBOutlet UIButton *actionsButton;

@property (strong, nonatomic) Tri2ViewController* viewer;
@property (assign, nonatomic) regina::Triangulation<2>* packet;
@end

@implementation Tri2Gluings

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Tri2ViewController*>(self.parentViewController);

    UITapGestureRecognizer *r = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touched:)];
    [self.triangles addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.triangles.delegate = self;
    self.triangles.dataSource = self;

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

    [self.viewer updateHeader:self.properties];
    self.orientButton.enabled = self.orientIcon.enabled = (self.packet->isOrientable() && ! self.packet->isOriented());
    [self.triangles reloadData];
}

+ (NSString*)destStringFromEdge:(int)srcEdge dest:(regina::Triangle<2>*)destTri gluing:(const regina::Perm<3>&)gluing
{
    if (! destTri)
        return @" "; // Use a space to ensure the label has enough height to pick up touches.
    else
        return [NSString stringWithFormat:@"%ld (%s)",
                destTri->markedIndex(),
                (gluing * regina::Edge<2>::ordering(srcEdge)).trunc2().c_str()];
}

- (IBAction)newTriangle:(id)sender {
    myEdit = YES;
    self.packet->newSimplex();
    myEdit = NO;

    // Update the necessary elements of the UI.
    NSIndexPath* last = [NSIndexPath indexPathForRow:self.packet->size()
                                          inSection:0];
    NSIndexPath* add = [NSIndexPath indexPathForRow:self.packet->size()+1
                                          inSection:0];
    [self.triangles insertRowsAtIndexPaths:@[last]
                          withRowAnimation:UITableViewRowAnimationAutomatic];
    [self.triangles scrollToRowAtIndexPath:add
                          atScrollPosition:UITableViewScrollPositionBottom animated:YES];

    [self.viewer updateHeader:self.properties];
}

- (void)endEditing
{
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [editField resignFirstResponder];
}

- (void)editGluingForSimplex:(int)simplex cell:(Tri2GluingCell*)cell label:(UILabel*)label
{
    editLabel = label;
    editSimplex = simplex;
    editEdge = label.tag;
    
    editField = [[UITextField alloc] initWithFrame:label.frame];
    editField.backgroundColor = cell.backgroundColor;
    editField.borderStyle = UITextBorderStyleNone;
    editField.placeholder = (editEdge >= 0 ? @"Gluing" : @"Name");
    editField.clearButtonMode = UITextFieldViewModeAlways;
    editField.returnKeyType = UIReturnKeyDone;
    editField.autocorrectionType = UITextAutocorrectionTypeNo;
    if (editEdge >= 0) {
        regina::Triangle<2>* t = self.packet->simplex(editSimplex);
        editField.text = [[Tri2Gluings destStringFromEdge:editEdge
                                                        dest:t->adjacentSimplex(editEdge)
                                                      gluing:t->adjacentGluing(editEdge)]
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
    
    UITapGestureRecognizer *tap = static_cast<UITapGestureRecognizer*>(sender);
    if (tap.state != UIGestureRecognizerStateRecognized)
        return;

    CGPoint location = [tap locationInView:self.triangles];
    NSIndexPath *indexPath = [self.triangles indexPathForRowAtPoint:location];
    if (indexPath.row == 0 || indexPath.row > self.packet->size())
        return;
    
    Tri2GluingCell* cell = static_cast<Tri2GluingCell*>([self.triangles cellForRowAtIndexPath:indexPath]);
    CGPoint inner = [self.triangles convertPoint:location toView:cell];
    if (CGRectContainsPoint(cell.index.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.index];
    else if (CGRectContainsPoint(cell.edge0.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.edge0];
    else if (CGRectContainsPoint(cell.edge1.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.edge1];
    else if (CGRectContainsPoint(cell.edge2.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.edge2];
}

#pragma mark - Triangulation operations

- (BOOL)checkEditable
{
    if (! self.packet->isPacketEditable()) {
        // 2-manifold triangulations are always editable... for now.
        // This should not happen.
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Cannot Change Gluings"
                                                        message:@"This should never happen - please report it to the Regina developers."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return NO;
    }

    return YES;
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

- (IBAction)doubleCover:(id)sender
{
    [self endEditing];
    if (! [self checkEditable])
        return;

    self.packet->makeDoubleCover();
}

- (IBAction)actions:(id)sender {
    UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                         destructiveButtonTitle:nil
                                              otherButtonTitles:@"Extract components",
                                                                @"Barycentric subdivision",
                                                                @"Double cover",
                                                                nil];
    [sheet showFromRect:self.actionsButton.frame inView:self.view animated:YES];
}

#pragma mark - Keyboard notifications

- (void)keyboardDidShow:(NSNotification*)notification
{
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;

    CGRect tableInDetail = [self.parentViewController.view convertRect:self.triangles.frame fromView:self.view];
    CGFloat unused = self.parentViewController.view.bounds.size.height - tableInDetail.origin.y - tableInDetail.size.height;

    if (kbSize.height <= unused)
        return;

    self.triangles.contentInset = UIEdgeInsetsMake(0, 0, kbSize.height - unused, 0);
    self.triangles.scrollIndicatorInsets = UIEdgeInsetsMake(0, 0, kbSize.height - unused, 0);

    [self.triangles scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:editSimplex+1 inSection:0]
                          atScrollPosition:UITableViewScrollPositionNone
                                  animated:YES];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    self.triangles.contentInset = UIEdgeInsetsZero;
    self.triangles.scrollIndicatorInsets = UIEdgeInsetsZero;
}

#pragma mark - Text field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    if (editField != textField) {
        NSLog(@"Error: Mismatched text field when editing gluings.");
        return;
    }
    
    regina::Triangle<2>* t = self.packet->simplex(editSimplex);
    
    NSMutableArray* toReload = [[NSMutableArray alloc] init];
    if (editEdge >= 0) {
        NSString* dest = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        if (dest.length == 0) {
            // We are making this a boundary edge.
            if (t->adjacentSimplex(editEdge)) {
                myEdit = YES;
                [toReload addObject:[NSIndexPath indexPathForRow:t->adjacentSimplex(editEdge)->markedIndex()+1 inSection:0]];
                t->unjoin(editEdge);
                editLabel.text = @" ";
                [self.viewer updateHeader:self.properties];
                myEdit = NO;
            }
        } else {
            NSRegularExpression* regex = [[NSRegularExpression alloc] initWithPattern:@"\\A(\\d+)[ ,\\(]+([0-2][0-2])[ \\)]*\\Z" options:0 error:nil];
            NSTextCheckingResult* result = [regex firstMatchInString:dest options:0 range:NSMakeRange(0, dest.length)];
            if (! result) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Gluing"
                                                                message:@"Please enter the gluing in the form triangle (edge).  For example, you could enter \"5 (20)\", or just \"5 20\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            int destSimplex = [[dest substringWithRange:[result rangeAtIndex:1]] intValue];
            if (destSimplex < 0 || destSimplex >= self.packet->size()) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Triangle"
                                                                message:@"Please enter the gluing in the form triangle (edge).  For example, you could enter \"5 (20)\", or just \"5 20\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }

            // We know at this point that we pass the regex, which means the adjacent edge
            // is in the form [0-2][0-2].
            int adjGluingAsInt = [dest substringWithRange:[result rangeAtIndex:2]].intValue;
            int adj0 = (adjGluingAsInt / 10);
            int adj1 = (adjGluingAsInt % 10);
            if (adj0 == adj1) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Edge"
                                                                message:@"Please enter the gluing in the form triangle (edge).  For example, you could enter \"5 (20)\", or just \"5 20\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            regina::Perm<3> destGluing = regina::Perm<3>(adj0, adj1, (3 - adj0 - adj1)) *
                regina::Edge<2>::ordering(editEdge).inverse();

            // Are we gluing the edge to itself?
            if (destSimplex == editSimplex && destGluing[editEdge] == editEdge) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cannot Glue an Edge to Itself"
                                                                message:nil
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            if (t->adjacentSimplex(editEdge) != self.packet->simplex(destSimplex) || destGluing != t->adjacentGluing(editEdge)) {
                // We are making a change, and it's a valid one.
                // Do it.
                myEdit = YES;
                {
                    regina::Packet::ChangeEventSpan span(self.packet);
                    
                    // First unglue from the old partner if it exists.
                    if (t->adjacentSimplex(editEdge)) {
                        [toReload addObject:[NSIndexPath indexPathForRow:t->adjacentSimplex(editEdge)->markedIndex()+1 inSection:0]];
                        t->unjoin(editEdge);
                    }
                    
                    // We are gluing the edge to a new partner.
                    int destEdge = destGluing[editEdge];
                    
                    // Does this new partner already have its own partner?
                    // If so, better unglue it.
                    regina::Triangle<2>* adj = self.packet->simplex(destSimplex);
                    if (adj->adjacentSimplex(destEdge)) {
                        NSIndexPath* path = [NSIndexPath indexPathForRow:adj->adjacentSimplex(destEdge)->markedIndex()+1 inSection:0];
                        if ([toReload indexOfObject:path] == NSNotFound)
                            [toReload addObject:path];
                        adj->unjoin(destEdge);
                    }
                    
                    // Glue the two edges together.
                    t->join(editEdge, adj, destGluing);
                    NSIndexPath* path = [NSIndexPath indexPathForRow:destSimplex+1 inSection:0];
                    if ([toReload indexOfObject:path] == NSNotFound)
                        [toReload addObject:path];
                    
                    editLabel.text = [NSString stringWithFormat:@"%d (%d%d)", destSimplex, adj0, adj1];
                }
                [self.viewer updateHeader:self.properties];
                myEdit = NO;
            }
        }
    } else {
        myEdit = YES;
        NSString* desc = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        t->setDescription(desc.UTF8String);
        editLabel.text = [NSString stringWithFormat:@"%d. %@", editSimplex, desc];
        myEdit = NO;
    }

cleanUpGluing:
    [editField removeFromSuperview];
    editField = nil;
    editLabel = nil;
    
    if (toReload.count)
        [self.triangles reloadRowsAtIndexPaths:toReload withRowAnimation:UITableViewRowAnimationFade];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 2 + self.packet->size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    else if (indexPath.row == self.packet->size() + 1)
        return [tableView dequeueReusableCellWithIdentifier:@"Add"];

    Tri2GluingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Triangle" forIndexPath:indexPath];
    regina::Triangle<2>* t = self.packet->simplex(indexPath.row - 1);
    cell.index.text = [NSString stringWithFormat:@"%d. %s", indexPath.row - 1, t->description().c_str()];
    cell.edge0.text = [Tri2Gluings destStringFromEdge:0 dest:t->adjacentSimplex(0) gluing:t->adjacentGluing(0)];
    cell.edge1.text = [Tri2Gluings destStringFromEdge:1 dest:t->adjacentSimplex(1) gluing:t->adjacentGluing(1)];
    cell.edge2.text = [Tri2Gluings destStringFromEdge:2 dest:t->adjacentSimplex(2) gluing:t->adjacentGluing(2)];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (indexPath.row > 0 && indexPath.row <= self.packet->size());
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0 || indexPath.row > self.packet->size())
        return;

    // Many rows could change - not only do we blank out gluings for adjacent triangles,
    // but we also need to reindex every triangle after the one that was removed.
    // Just reload everything.
    // This is easy: we don't set myEdit=YES, and instead rely on the automatic packet reload.
    self.packet->removeSimplexAt(indexPath.row - 1);
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.triangles.rowHeight;

    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [self.triangles dequeueReusableCellWithIdentifier:@"Header"];
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
    // try to open any other popover.
    switch (buttonIndex) {
        case 0:
            [self extractComponents:nil]; break;
        case 1:
            [self barycentricSubdivision:nil]; break;
        case 2:
            [self doubleCover:nil]; break;
    }
}

@end
