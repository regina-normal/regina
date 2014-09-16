
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

#import "EditableTableViewController.h"

@interface EditableTableViewController () <UITextFieldDelegate> {
    UIEdgeInsets _originalContentInsets;
    UIEdgeInsets _originalScrollIndicatorInsets;
}
@end

@implementation EditableTableViewController

- (void)viewDidLoad
{
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.tableView addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

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

- (IBAction)longPress:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    UIGestureRecognizerState state = press.state;

    CGPoint location = [press locationInView:self.tableView];
    NSIndexPath *indexPath = [self.tableView indexPathForRowAtPoint:location];

    if (self.actionPath) {
        // We are in the middle of one action; do not start another.
        return;
    }

    if (indexPath && [self renameAllowed:indexPath]) {
        if (state == UIGestureRecognizerStateBegan) {
            self.actionPath = indexPath;

            UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];

            // Create a background sheet to hide the old cell contents.
            // The inset of dy=1 is so we don't cover the cell dividers.
            UIView* bg = [[UIView alloc] initWithFrame:CGRectInset(cell.frame, 0, 1)];
            bg.backgroundColor = cell.backgroundColor;

            // The text field itself will be smaller.
            // Add a small additional y inset, and an x inset that aligns the text field
            // with the labels in other cells.
            CGRect frame = CGRectMake(0, 0, CGRectGetWidth(bg.frame), CGRectGetHeight(bg.frame));
            frame = CGRectInset(frame, CGRectGetMinX(cell.textLabel.frame), 5);

            UITextField* f = [[UITextField alloc] initWithFrame:frame];
            f.backgroundColor = cell.backgroundColor;
            f.borderStyle = UITextBorderStyleRoundedRect;
            // f.placeholder = @"Type your document name...";
            f.clearButtonMode = UITextFieldViewModeAlways;
            f.text = [self renameInit:indexPath];
            f.returnKeyType = UIReturnKeyDone;
            f.autocapitalizationType = UITextAutocapitalizationTypeSentences;
            f.autocorrectionType = UITextAutocorrectionTypeNo;
            f.delegate = self;

            [bg addSubview:f];
            [self.tableView addSubview:bg];
            [f becomeFirstResponder];
        }
    }
}

- (BOOL)renameAllowed:(NSIndexPath *)path
{
    return NO;
}

- (NSString *)renameInit:(NSIndexPath *)path
{
    return nil;
}

- (void)renameDone:(NSIndexPath *)path result:(NSString *)result
{
}

- (void)keyboardDidShow:(NSNotification*)notification
{
    _originalContentInsets = self.tableView.contentInset;
    _originalScrollIndicatorInsets = self.tableView.scrollIndicatorInsets;

    // TODO: kbHeight is still not right in portrait mode.
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    CGFloat kbHeight = UIDeviceOrientationIsPortrait([[UIDevice currentDevice] orientation]) ?
        kbSize.height : kbSize.width;

    self.tableView.contentInset = UIEdgeInsetsMake(_originalContentInsets.top,
                                                   _originalContentInsets.left,
                                                   kbHeight,
                                                   _originalContentInsets.right);

    self.tableView.scrollIndicatorInsets = UIEdgeInsetsMake(_originalScrollIndicatorInsets.top,
                                                            _originalScrollIndicatorInsets.left,
                                                            kbHeight,
                                                            _originalScrollIndicatorInsets.right);

    if (self.actionPath)
        [self.tableView scrollToRowAtIndexPath:self.actionPath
                              atScrollPosition:UITableViewScrollPositionNone
                                      animated:YES];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    self.tableView.contentInset = _originalContentInsets;
    self.tableView.scrollIndicatorInsets = _originalScrollIndicatorInsets;
}

#pragma mark - Text Field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    NSIndexPath* renamePath = self.actionPath;
    NSString* text = textField.text;

    // The superview of the text field is the empty sheet covering up the old cell contents.
    // Remove that sheet (and the text field it contains) from the table view.
    [textField.superview removeFromSuperview];
    self.actionPath = nil;

    [self renameDone:renamePath result:text];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

@end
