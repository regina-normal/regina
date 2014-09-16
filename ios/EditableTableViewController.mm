
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

@interface EditableTableViewController () {
    UIEdgeInsets _originalContentInsets;
    UIEdgeInsets _originalScrollIndicatorInsets;
}
@end

@implementation EditableTableViewController

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

- (void)keyboardDidShow:(NSNotification*)notification
{
    _originalContentInsets = self.tableView.contentInset;
    _originalScrollIndicatorInsets = self.tableView.scrollIndicatorInsets;

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

@end
