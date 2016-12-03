
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
#import "SnapPeaViewController.h"
#import "SnapPeaCusps.h"
#import "maths/numbertheory.h"
#import "snappea/snappeatriangulation.h"

#pragma mark - Table cells

@interface SnapPeaCuspCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *cusp;
@property (weak, nonatomic) IBOutlet UILabel *vertex;
@property (weak, nonatomic) IBOutlet UILabel *filling;
@end

@implementation SnapPeaCuspCell
@end

@interface SnapPeaShapeCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *real;
@property (weak, nonatomic) IBOutlet UILabel *imag;
@end

@implementation SnapPeaShapeCell
@end

#pragma mark - SnapPea cusps

@interface SnapPeaCusps () <UITableViewDataSource, UITableViewDelegate, UITextFieldDelegate> {
    CGFloat headerHeight;
    UILabel* editLabel;
    UITextField* editField;
    int editCusp;
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UITableView *cusps;
@property (weak, nonatomic) IBOutlet UITableView *shapes;
@property (weak, nonatomic) IBOutlet UIButton *fill;
@property (weak, nonatomic) IBOutlet UIButton *fillIcon;

@property (strong, nonatomic) SnapPeaViewController* viewer;
@property (assign, nonatomic) regina::SnapPeaTriangulation* packet;
@end

@implementation SnapPeaCusps

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<SnapPeaViewController*>(self.parentViewController);
    
    UITapGestureRecognizer *r = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touched:)];
    [self.cusps addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;
    
    self.cusps.delegate = self;
    self.cusps.dataSource = self;
    
    self.shapes.delegate = self;
    self.shapes.dataSource = self;
    
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
    
    [self.viewer updateHeader:self.header volume:self.volume solnType:self.solnType];

    self.fill.enabled = self.fillIcon.enabled = (self.packet->countFilledCusps() > 0);
    [self.cusps reloadData];
    [self.shapes reloadData];
}

- (void)endEditing
{
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [editField resignFirstResponder];
}

- (IBAction)fillCusps:(id)sender
{
    regina::Triangulation<3>* s = self.packet->filledTriangulation();
    s->setLabel(self.packet->adornedLabel("Filled"));
    self.packet->insertChildLast(s);
    [ReginaHelper viewPacket:s];
}

- (void)editFillingForCusp:(int)cusp cell:(SnapPeaCuspCell*)cell label:(UILabel*)label
{
    editLabel = label;
    editCusp = cusp;
    
    const regina::Cusp* c = self.packet->cusp(cusp);
    
    editField = [[UITextField alloc] initWithFrame:label.frame];
    editField.backgroundColor = cell.backgroundColor;
    editField.borderStyle = UITextBorderStyleNone;
    editField.placeholder = @"Filling";
    editField.clearButtonMode = UITextFieldViewModeAlways;
    editField.returnKeyType = UIReturnKeyDone;
    editField.autocorrectionType = UITextAutocorrectionTypeNo;
    if (! c->complete())
        editField.text = [NSString stringWithFormat:@"%d, %d", c->m(), c->l()];
    editField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    editField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
    editField.textAlignment = NSTextAlignmentLeft;
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
    
    CGPoint location = [tap locationInView:self.cusps];
    NSIndexPath *indexPath = [self.cusps indexPathForRowAtPoint:location];
    if (indexPath.row == 0 || indexPath.row > self.packet->countCusps())
        return;
    
    SnapPeaCuspCell* cell = static_cast<SnapPeaCuspCell*>([self.cusps cellForRowAtIndexPath:indexPath]);
    CGPoint inner = [self.cusps convertPoint:location toView:cell];
    if (CGRectContainsPoint(cell.filling.frame, inner))
        [self editFillingForCusp:indexPath.row-1 cell:cell label:cell.filling];
}

#pragma mark - Keyboard notifications

- (void)keyboardDidShow:(NSNotification*)notification
{
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;

    CGRect tableInDetail = [self.parentViewController.view convertRect:self.cusps.bounds fromView:self.cusps];
    CGFloat unused = self.parentViewController.view.bounds.size.height - tableInDetail.origin.y - tableInDetail.size.height;

    if (kbSize.height <= unused)
        return;

    self.cusps.contentInset = UIEdgeInsetsMake(0, 0, kbSize.height - unused, 0);
    self.cusps.scrollIndicatorInsets = UIEdgeInsetsMake(0, 0, kbSize.height - unused, 0);

    [self.cusps scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:editCusp+1 inSection:0]
                      atScrollPosition:UITableViewScrollPositionNone
                              animated:YES];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    self.cusps.contentInset = UIEdgeInsetsZero;
    self.cusps.scrollIndicatorInsets = UIEdgeInsetsZero;
}

#pragma mark - Text field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    if (editField != textField) {
        NSLog(@"Error: Mismatched text field when editing fillings.");
        return;
    }

    BOOL reload = NO;
    
    NSString* filling = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if (filling.length == 0 || [filling isEqualToString:@"-"]) {
        // We are making this cusp complete.
        if (! self.packet->cusp(editCusp)->complete()) {
            myEdit = YES;
            self.packet->unfill(editCusp);
            editLabel.text = @"—";
            reload = YES;
            myEdit = NO;
        }
    } else {
        NSRegularExpression* regex = [[NSRegularExpression alloc] initWithPattern:@"\\A(-?\\d+)[ ,]+(-?\\d+)\\Z" options:0 error:nil];
        NSTextCheckingResult* result = [regex firstMatchInString:filling options:0 range:NSMakeRange(0, filling.length)];
        if (! result) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Filling"
                                                            message:@"Please enter a pair of filling coefficients.  For example, you could enter \"5, 3\", or just \"5 3\".  To remove a filling, just leave this field blank."
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            goto cleanUpFilling;
        }
        
        int m = [[filling substringWithRange:[result rangeAtIndex:1]] intValue];
        int l = [[filling substringWithRange:[result rangeAtIndex:2]] intValue];

        if (m == INT_MAX || m == INT_MIN || l == INT_MAX || l == INT_MIN) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Filling Coefficients Too Large"
                                                            message:nil
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            goto cleanUpFilling;
        }
        
        if (m == 0 && l == 0) {
            // Again, make this cusp complete.
            if (! self.packet->cusp(editCusp)->complete()) {
                myEdit = YES;
                self.packet->unfill(editCusp);
                editLabel.text = @"—";
                reload = YES;
                myEdit = NO;
            }
            goto cleanUpFilling;
        }
        
        if (regina::gcd(m, l) != 1) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Filling Coefficients Must Be Coprime"
                                                            message:nil
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            goto cleanUpFilling;
        }

        if ((! self.packet->cusp(editCusp)->vertex()->isLinkOrientable()) && l != 0) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Non-Orientable Filling Coefficients Must Be (±1, 0)"
                                                            message:nil
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            goto cleanUpFilling;
        }
        
        myEdit = YES;
        if (! self.packet->fill(m, l, editCusp)) {
            myEdit = NO;
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could Not Fill"
                                                            message:@"SnapPea rejected these filling coefficients, and I'm not sure why."
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            goto cleanUpFilling;
        }
        editLabel.text = [NSString stringWithFormat:@"%d, %d", m, l];
        reload = YES;
        myEdit = NO;
    }
    
cleanUpFilling:
    [editField removeFromSuperview];
    editField = nil;
    editLabel = nil;
    
    if (reload) {
        [self.viewer updateHeader:self.header volume:self.volume solnType:self.solnType];
        [self.shapes reloadData];
        self.fill.enabled = self.fillIcon.enabled = (self.packet->countFilledCusps() > 0);
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

#pragma mark - Table view

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    
    if (tableView == self.cusps) {
        SnapPeaCuspCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cusp" forIndexPath:indexPath];
        const regina::Cusp* cusp = self.packet->cusp(indexPath.row - 1);
        cell.cusp.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
        cell.vertex.text = [NSString stringWithFormat:@"%ld", cusp->vertex()->markedIndex()];
        if (cusp->complete())
            cell.filling.text = @"—";
        else
            cell.filling.text = [NSString stringWithFormat:@"%d, %d", cusp->m(), cusp->l()];
        return cell;
    } else {
        SnapPeaShapeCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Shape" forIndexPath:indexPath];
        std::complex<double> shape = self.packet->shape(indexPath.row - 1);
        cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
        cell.real.text = [NSString stringWithFormat:@"%g", shape.real()];
        cell.imag.text = [NSString stringWithFormat:@"%g", shape.imag()];
        return cell;
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return tableView.rowHeight;
    
    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1 + (tableView == self.cusps ? self.packet->countCusps() : self.packet->size());
}

@end
