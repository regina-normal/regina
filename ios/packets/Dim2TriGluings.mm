
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

#import "Dim2TriGluings.h"
#import "Dim2TriangulationViewController.h"
#import "dim2/dim2triangulation.h"

// TODO: Edit gluings!  Don't forget the keyboard scrolling nonsense.
// TODO: A few deletes and then we get stuck.
// TODO: Extend height of tap region to the entire cell.

#pragma mark - Table cell

@interface Dim2GluingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *edge2;
@property (weak, nonatomic) IBOutlet UILabel *edge1;
@property (weak, nonatomic) IBOutlet UILabel *edge0;
@end

@implementation Dim2GluingCell
@end

#pragma mark - Dim2Triangulation gluings viewer

@interface Dim2TriGluings () <UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate> {
    CGFloat headerHeight;
    UITextField* editField;
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UILabel *properties;
@property (weak, nonatomic) IBOutlet UITableView *triangles;

@property (strong, nonatomic) Dim2TriangulationViewController* viewer;
@property (assign, nonatomic) regina::Dim2Triangulation* packet;
@end

@implementation Dim2TriGluings

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Dim2TriangulationViewController*>(self.parentViewController);

    UITapGestureRecognizer *r = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touched:)];
    [self.triangles addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.triangles.delegate = self;
    self.triangles.dataSource = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    if (myEdit)
        return;

    [self.viewer updateHeader:self.properties];
    [self.triangles reloadData];
}

+ (NSString*)destStringFromEdge:(int)srcEdge dest:(regina::Dim2Triangle*)destTri gluing:(const regina::NPerm3&)gluing
{
    if (! destTri)
        return @" "; // Use a space to ensure the label has enough height to pick up touches.
    else
        return [NSString stringWithFormat:@"%ld (%s)",
                destTri->markedIndex(),
                (gluing * regina::Dim2Edge::ordering[srcEdge]).trunc2().c_str()];
}

- (IBAction)newTriangle:(id)sender {
    myEdit = YES;
    self.packet->newSimplex();
    myEdit = NO;

    // Update the necessary elements of the UI.
    NSIndexPath* last = [NSIndexPath indexPathForRow:self.packet->getNumberOfSimplices()
                                          inSection:0];
    NSIndexPath* add = [NSIndexPath indexPathForRow:self.packet->getNumberOfSimplices()+1
                                          inSection:0];
    [self.triangles insertRowsAtIndexPaths:@[last]
                          withRowAnimation:UITableViewRowAnimationAutomatic];
    [self.triangles scrollToRowAtIndexPath:add
                          atScrollPosition:UITableViewScrollPositionBottom animated:YES];

    [self.viewer updateHeader:self.properties];
}

- (void)endEditing
{
    // TODO: Check that this works.
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [editField resignFirstResponder];
}

- (void)editGluing:(int)simplex edge:(int)edge cell:(Dim2GluingCell*)cell label:(UILabel*)label
{
    // Finish and process any other edit that is currently in progress.
    if (editField)
        [editField resignFirstResponder];
    
    NSLog(@"New edit field");
    editField = [[UITextField alloc] initWithFrame:label.frame];
    editField.backgroundColor = cell.backgroundColor;
    editField.borderStyle = UITextBorderStyleNone;
    editField.placeholder = @"Gluing";
    editField.clearButtonMode = UITextFieldViewModeAlways;
    editField.text = label.text;
    editField.returnKeyType = UIReturnKeyDone;
    editField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    editField.autocorrectionType = UITextAutocorrectionTypeNo;
    editField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
    editField.textAlignment = NSTextAlignmentRight;
    editField.delegate = self;

    [cell addSubview:editField];
    [editField becomeFirstResponder];
}

- (IBAction)touched:(id)sender {
    UITapGestureRecognizer *tap = static_cast<UITapGestureRecognizer*>(sender);
    if (tap.state != UIGestureRecognizerStateRecognized)
        return;

    CGPoint location = [tap locationInView:self.triangles];
    NSIndexPath *indexPath = [self.triangles indexPathForRowAtPoint:location];
    if (indexPath.row == 0 || indexPath.row > self.packet->getNumberOfSimplices())
        return;

    Dim2GluingCell* cell = static_cast<Dim2GluingCell*>([self.triangles cellForRowAtIndexPath:indexPath]);
    CGPoint inner = [self.triangles convertPoint:location toView:cell];
    if (CGRectContainsPoint(cell.index.frame, inner)) {
        // TODO: Relabel the simplex.
    } else if (CGRectContainsPoint(cell.edge0.frame, inner))
        [self editGluing:indexPath.row-1 edge:0 cell:cell label:cell.edge0];
    else if (CGRectContainsPoint(cell.edge1.frame, inner))
        [self editGluing:indexPath.row-1 edge:1 cell:cell label:cell.edge1];
    else if (CGRectContainsPoint(cell.edge2.frame, inner))
        [self editGluing:indexPath.row-1 edge:2 cell:cell label:cell.edge2];
}

#pragma mark - Text field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    NSLog(@"TODO: New gluing: %@", textField.text);

    [textField removeFromSuperview];
    if (textField == editField)
        editField = nil;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 2 + self.packet->getNumberOfTriangles();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    else if (indexPath.row == self.packet->getNumberOfTriangles() + 1)
        return [tableView dequeueReusableCellWithIdentifier:@"Add"];

    Dim2GluingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Triangle" forIndexPath:indexPath];
    regina::Dim2Triangle* t = self.packet->getTriangle(indexPath.row - 1);
    cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
    // TODO: Show triangle name.
    cell.edge0.text = [Dim2TriGluings destStringFromEdge:0 dest:t->adjacentTriangle(0) gluing:t->adjacentGluing(0)];
    cell.edge1.text = [Dim2TriGluings destStringFromEdge:1 dest:t->adjacentTriangle(1) gluing:t->adjacentGluing(1)];
    cell.edge2.text = [Dim2TriGluings destStringFromEdge:2 dest:t->adjacentTriangle(2) gluing:t->adjacentGluing(2)];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (indexPath.row > 0 && indexPath.row <= self.packet->getNumberOfSimplices());
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0 || indexPath.row > self.packet->getNumberOfSimplices())
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

@end
