
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

#import "PacketManagerIOS.h"
#import "ScriptViewController.h"
#import "packet/script.h"
#import "syntax/repository.h"

#import "../python/PythonConsoleController.h"
#import "../python/SyntaxHighlighter.h"

// The syntax highlighting repository of definitions and themes is
// a singleton: it is created on demand, and never deleted.
regina::syntax::Repository* repository;

#pragma mark - Script variable cell

@interface ScriptVariableCell ()
@property (weak, nonatomic) IBOutlet UILabel *variable;
@property (weak, nonatomic) IBOutlet UILabel *value;
@property (weak, nonatomic) IBOutlet UIImageView *icon;
@end

@implementation ScriptVariableCell
@end

#pragma mark - Script view controller

@interface ScriptViewController () <UITableViewDataSource, UITableViewDelegate, UITextViewDelegate, NSTextStorageDelegate> {
    CGFloat variableHeaderHeight;
    SyntaxHighlighter* highlighter;
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UITableView *variables;
@property (weak, nonatomic) IBOutlet UITextView *script;
@property (assign, nonatomic) regina::Script* packet;
@end

@implementation ScriptViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    variableHeaderHeight = 0;

    self.variables.delegate = self;
    self.variables.dataSource = self;

    if (! repository)
        repository = new regina::syntax::Repository;

    highlighter = [[SyntaxHighlighter alloc] init];
    highlighter.definition = repository->definitionForName("Python");
    highlighter.theme = repository->theme("Default");

    self.script.delegate = self;
    self.script.textStorage.delegate = highlighter;

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

    self.script.text = [NSString stringWithUTF8String:self.packet->text().c_str()];
    [self.script scrollRangeToVisible:NSMakeRange(0, 0)];
    [self.variables reloadData];
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
    myEdit = YES;
    self.packet->setText([textView.text UTF8String]);
    myEdit = NO;
}

- (void)ensureCursorVisible
{
    // Sigh.  If we call [self.detail scrollRangeToVisible:self.detail.selectedRange],
    // the cursor does not move up above the keyboard (happens in both iOS 7 and iOS 8).
    // Scroll manually instead.
    CGRect caret = [self.script caretRectForPosition:self.script.selectedTextRange.end];
    CGFloat overflow = caret.origin.y + caret.size.height - (self.script.contentOffset.y + self.script.bounds.size.height - self.script.contentInset.bottom /* keyboard height */);
    if (overflow > 0) {
        // Add a few points more than the required offset.
        [UIView animateWithDuration:.1 animations:^{
            self.script.contentOffset = CGPointMake(self.script.contentOffset.x,
                                                    self.script.contentOffset.y + overflow + 7);
        }];
    }
}

- (void)keyboardDidShow:(NSNotification*)notification
{
    // Find the gap between the text view and the bottom of the screen.
    CGFloat gap = self.script.superview.frame.size.height - self.script.frame.origin.y - self.script.frame.size.height;

    // On iOS 8 (but not all orientations for iOS 7), what we need is kbSize.height.
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;

    self.script.contentInset = UIEdgeInsetsMake(0, 0, kbSize.height - gap, 0);
    self.script.scrollIndicatorInsets = UIEdgeInsetsMake(0, 0, kbSize.height - gap, 0);

    [self ensureCursorVisible];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    self.script.contentInset = UIEdgeInsetsZero;
    self.script.scrollIndicatorInsets = UIEdgeInsetsZero;
}

- (IBAction)newVariable:(id)sender {
    self.packet->addVariableName("var", nil);
    // TODO: Scroll to visible
}

- (IBAction)run:(id)sender {
    [PythonConsoleController openConsoleFromViewController:self root:nil item:nil script:self.packet];
}

#pragma mark - Table view

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];

    /*
    if (self.packet->countVariables() == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Empty"];
     */

    if (indexPath.row == self.packet->countVariables() + 1)
        return [tableView dequeueReusableCellWithIdentifier:@"Add"];

    ScriptVariableCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Variable" forIndexPath:indexPath];
    cell.variable.text = [NSString stringWithUTF8String:self.packet->variableName(indexPath.row - 1).c_str()];

    regina::Packet* value = self.packet->variableValue(indexPath.row - 1);
    if (value) {
        cell.icon.image = [PacketManagerIOS iconFor:value];
        if (value->parent())
            cell.value.text = [NSString stringWithUTF8String:value->label().c_str()];
        else
            cell.value.text = @"⟨Entire tree⟩";
    } else {
        cell.icon.image = nil;
        cell.value.text = @"⟨None⟩";
    }
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    unsigned long nVar = self.packet->countVariables();
    return 2 + nVar;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (indexPath.row > 0 && indexPath.row <= self.packet->countVariables());
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0 || indexPath.row > self.packet->countVariables())
        return;

    // For now, we rely on the automatic packet reload.
    // TODO: Set myEdit and instead reload just the section of the table that changed.
    self.packet->removeVariable(indexPath.row - 1);
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.variables.rowHeight;

    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (variableHeaderHeight == 0) {
        UITableViewCell* cell = [self.variables dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        variableHeaderHeight = size.height;
    }
    return variableHeaderHeight;
}

@end
