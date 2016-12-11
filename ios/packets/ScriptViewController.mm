
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
#import "PacketPicker.h"
#import "ReginaHelper.h"
#import "ScriptViewController.h"
#import "packet/script.h"
#import "syntax/repository.h"

#import "../python/PythonConsoleController.h"
#import "../python/SyntaxHighlighter.h"

// The syntax highlighting repository of definitions and themes is
// a singleton: it is created on demand, and never deleted.
regina::syntax::Repository* repository;

#pragma mark - Script variable value field

@implementation ScriptVariableValueField

- (CGRect)caretRectForPosition:(UITextPosition *)position
{
    // Hide the blinking cursor.
    return CGRectZero;
}

@end

#pragma mark - Script variable cell

@interface ScriptVariableCell () <UITextFieldDelegate, PacketPickerWatcher> {
    std::string _name;
    regina::Packet* _packet;
    regina::Packet* _tree;
}
// To initialise the contents of a cell, call initVariable.
// These will fill the UI elements with appropriate contents - there is no need
// to fill the UI elements variable, value and icon directly.
@property (weak, nonatomic) IBOutlet UITextField *variable;
@property (weak, nonatomic) IBOutlet UITextField *value;
@property (weak, nonatomic) IBOutlet UIImageView *icon;
@property (weak, nonatomic) ScriptViewController *scriptView;
@end

@implementation ScriptVariableCell

- (void)updateValueDisplay
{
    if (_packet) {
        self.icon.image = [PacketManagerIOS iconFor:_packet];
        if (_packet->parent())
            self.value.text = [NSString stringWithUTF8String:_packet->label().c_str()];
        else
            self.value.text = @"⟨Entire tree⟩";
    } else {
        self.icon.image = nil;
        self.value.text = @"⟨None⟩";
    }
}

- (void)initVariable:(const std::string&)name value:(regina::Packet*)packet tree:(regina::Packet*)tree
{
    _name = name;
    _packet = packet;
    _tree = tree;

    self.variable.text = [NSString stringWithUTF8String:_name.c_str()];
    [self updateValueDisplay];

    self.value.delegate = self;
}

- (IBAction)finishEditing:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)endEditName:(id)sender {
    // TODO: Implement
    // TODO: Include sorting in table, and checking that this looks like a variable name.
}

- (IBAction)beginEditValue:(id)sender {
    PacketPicker* p = [[PacketPicker alloc] init];
    [p fill:_tree allowNone:YES noneText:@"⟨None⟩" allowRoot:YES rootText:@"⟨Entire tree⟩" select:_packet];
    p.watcher = self;
    self.value.inputView = p;

    UIBarButtonItem* done = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                          target:self.value
                                                                          action:@selector(resignFirstResponder)];

    // TODO: Fix the baseline movement on edit.
    if ([ReginaHelper ios9]) {
        UIBarButtonItemGroup* group = [[UIBarButtonItemGroup alloc] initWithBarButtonItems:@[done] representativeItem:nil];
        self.value.inputAssistantItem.leadingBarButtonGroups = @[];
        self.value.inputAssistantItem.trailingBarButtonGroups = @[group];
    } else {
        UIToolbar* bar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, self.scriptView.view.frame.size.width, 50)];
        bar.barStyle = UIBarStyleDefault;
        UIBarButtonItem* gap = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
        bar.items = @[gap, done];
        [bar sizeToFit];
        self.value.inputAccessoryView = bar;
    }
}

- (IBAction)endEditValue:(id)sender {
    self.value.inputView = nil;
}

- (void)packetPicker:(PacketPicker *)picker selected:(regina::Packet *)packet
{
    if (packet != _packet) {
        _packet = packet;
        [self.scriptView editedValue:_name value:packet];
        [self updateValueDisplay];
    }
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    // Disable external keyboards, clipboard actions, etc.
    return NO;
}

@end

#pragma mark - Script view controller

@interface ScriptViewController () <UITableViewDataSource, UITableViewDelegate, UITextViewDelegate> {
    CGFloat variableHeaderHeight;
    SyntaxHighlighter* highlighter;
    BOOL myEdit;
    regina::Packet* tree;
}
@property (weak, nonatomic) IBOutlet UITableView *variables;
@property (weak, nonatomic) IBOutlet UITextView *script;
@property (assign, nonatomic) regina::Script* packet;
@end

@implementation ScriptViewController

- (void)setPacket:(regina::Script *)packet
{
    _packet = packet;
    tree = packet->root();
}

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

- (NSIndexPath*)pathForVariable:(const std::string&)name {
    long index = self.packet->variableIndex(name);
    if (index < 0)
        return nil;
    return [NSIndexPath indexPathForRow:(index + 1) inSection:0];
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
    myEdit = true;
    NSIndexPath* path = [self pathForVariable:self.packet->addVariableName("var", nil)];
    myEdit = false;

    if (path) {
        [self.variables insertRowsAtIndexPaths:@[path] withRowAnimation:UITableViewRowAnimationAutomatic];
        [self.variables scrollToRowAtIndexPath:path atScrollPosition:UITableViewScrollPositionNone animated:YES];
    }
}

- (void)editedValue:(const std::string&)name value:(regina::Packet*)value
{
    myEdit = true;
    long index = self.packet->variableIndex(name);
    if (index >= 0)
        self.packet->setVariableValue(index, value);
    myEdit = false;
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
    cell.scriptView = self;
    [cell initVariable:self.packet->variableName(indexPath.row - 1)
                 value:self.packet->variableValue(indexPath.row - 1)
                  tree:tree];
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

    myEdit = true;
    self.packet->removeVariable(indexPath.row - 1);
    myEdit = false;
    [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
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
