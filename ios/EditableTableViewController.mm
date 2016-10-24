
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

#import "EditableTableViewController.h"
#import "ReginaHelper.h"

@interface EditableTableViewController () {
    UITextField* renameField;
    NSInteger renameIndex;
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
}

- (void)viewWillDisappear:(BOOL)animated
{
    if (renameField)
        [renameField endEditing:YES];
    
    [super viewWillDisappear:animated];

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc removeObserver:self name:UIKeyboardDidShowNotification object:nil];
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

            NSArray* other = self.otherActionLabels;
            if (other) {
                // Show an action sheet offering the various actions available.
                UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:self.actionPath];

                UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil
                                                                               message:nil
                                                                        preferredStyle:UIAlertControllerStyleActionSheet];

                int index = 0;
                for (NSString* label : other) {
                    [alert addAction:[UIAlertAction actionWithTitle:label
                                                              style:UIAlertActionStyleDefault
                                                            handler:^(UIAlertAction*){
                                                                        [self otherActionSelected:index];
                                                                        self.actionPath = nil;
                                                                    }]];
                    ++index;
                }
                [alert addAction:[UIAlertAction actionWithTitle:@"Rename"
                                                          style:UIAlertActionStyleDefault
                                                        handler:^(UIAlertAction*) {
                                                                    [self renameBegin];
                                                                }]];
                [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                          style:UIAlertActionStyleCancel
                                                        handler:^(UIAlertAction*) {
                                                                    self.actionPath = nil;
                                                                }]];
                [alert setModalPresentationStyle:UIModalPresentationPopover];
                alert.popoverPresentationController.sourceView = self.tableView;
                alert.popoverPresentationController.sourceRect = cell.frame;
                [self presentViewController:alert animated:YES completion:nil];
            } else {
                // The only available action is rename.
                // Just do it immediately.
                [self renameBegin];
            }
        }
    }
}

- (void)renameBegin
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:self.actionPath];

    // Create a background sheet to hide the old cell contents.
    // The inset of dy=1 is so we don't cover the cell dividers.
    UIView* bg = [[UIView alloc] initWithFrame:CGRectInset(cell.frame, 0, 1)];
    bg.backgroundColor = cell.backgroundColor;

    // The text field itself will be smaller.
    // Add a small additional y inset, and an x inset that aligns the text field
    // with the labels in other cells.
    CGRect frame = CGRectMake(0, 0, CGRectGetWidth(bg.frame), CGRectGetHeight(bg.frame));
    frame = CGRectInset(frame, CGRectGetMinX(cell.textLabel.frame), 5);

    renameField = [[UITextField alloc] initWithFrame:frame];
    renameField.backgroundColor = cell.backgroundColor;
    renameField.borderStyle = UITextBorderStyleRoundedRect;
    // renameField.placeholder = @"Type your document name...";
    renameField.clearButtonMode = UITextFieldViewModeAlways;
    renameField.text = [self renameInit:self.actionPath];
    renameField.returnKeyType = UIReturnKeyDone;
    renameField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
    renameField.autocorrectionType = UITextAutocorrectionTypeNo;
    renameField.delegate = self;

    [bg addSubview:renameField];
    [self.tableView addSubview:bg];
    [renameField becomeFirstResponder];
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

- (NSArray *)otherActionLabels
{
    return nil;
}

- (void)otherActionSelected:(int)which
{
}

- (NSString *)deleteConfirmation:(NSIndexPath *)path
{
    return @"Delete";
}

- (void)deleteAction
{
}

- (void)keyboardDidShow:(NSNotification*)notification
{
    if (self.actionPath)
        [self.tableView scrollToRowAtIndexPath:self.actionPath
                              atScrollPosition:UITableViewScrollPositionNone
                                      animated:YES];
}

#pragma mark - Table view

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (self.actionPath)
        return;

    if (editingStyle == UITableViewCellEditingStyleDelete) {
        self.actionPath = indexPath;
        CGRect cell = [tableView cellForRowAtIndexPath:indexPath].frame;

        UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil
                                                                       message:nil
                                                                preferredStyle:UIAlertControllerStyleActionSheet];
        [alert addAction:[UIAlertAction actionWithTitle:[self deleteConfirmation:indexPath]
                                                  style:UIAlertActionStyleDestructive
                                                handler:^(UIAlertAction*) {
                                                            [self deleteAction];
                                                            self.actionPath = nil;
                                                        }]];
        [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                  style:UIAlertActionStyleCancel
                                                handler:^(UIAlertAction*) {
                                                            self.actionPath = nil;
                                                        }]];
        [alert setModalPresentationStyle:UIModalPresentationPopover];
        alert.popoverPresentationController.sourceView = tableView;
        alert.popoverPresentationController.sourceRect = cell;
        // Popovers to the right appear strangely cropped on iOS 8.  Disallow them.
        alert.popoverPresentationController.permittedArrowDirections = UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown;
        [self presentViewController:alert animated:YES completion:nil];
    }
}

#pragma mark - Text field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    NSIndexPath* renamePath = self.actionPath;
    NSString* text = textField.text;

    // The superview of the text field is the empty sheet covering up the old cell contents.
    // Remove that sheet (and the text field it contains) from the table view.
    [textField.superview removeFromSuperview];
    self.actionPath = nil;
    renameField = nil;

    [self renameDone:renamePath result:text];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

@end
