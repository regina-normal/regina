
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
#import "TextViewController.h"
#import "packet/text.h"

@interface TextViewController () <UITextViewDelegate> {
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UITextView *detail;
@property (assign, nonatomic) regina::Text* packet;
@end

@implementation TextViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.detail.delegate = self;

    [self reloadPacket];
    [self.detail setSelectedRange:NSMakeRange(0,0)];

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

- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];

    // TODO: This doesn't work when the device is rotated.
    [self.detail scrollRangeToVisible:self.detail.selectedRange];
}

- (void)reloadPacket {
    if (myEdit)
        return;

    self.detail.text = [NSString stringWithUTF8String:self.packet->text().c_str()];
}

- (void)endEditing
{
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [self.detail resignFirstResponder];
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
    CGRect caret = [self.detail caretRectForPosition:self.detail.selectedTextRange.end];
    CGFloat overflow = caret.origin.y + caret.size.height - (self.detail.contentOffset.y + self.detail.bounds.size.height - self.detail.contentInset.bottom /* keyboard height */);
    if (overflow > 0) {
        // Add a few points more than the required offset.
        [UIView animateWithDuration:.1 animations:^{
            self.detail.contentOffset = CGPointMake(self.detail.contentOffset.x,
                                                    self.detail.contentOffset.y + overflow + 7);
        }];
    }
}

- (void)keyboardDidShow:(NSNotification*)notification
{
    // On iOS 8 (but not all orientations for iOS 7), what we need is kbSize.height.
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;

    self.detail.contentInset = UIEdgeInsetsMake(0, 0, kbSize.height, 0);
    self.detail.scrollIndicatorInsets = UIEdgeInsetsMake(0, 0, kbSize.height, 0);

    [self ensureCursorVisible];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    self.detail.contentInset = UIEdgeInsetsZero;
    self.detail.scrollIndicatorInsets = UIEdgeInsetsZero;
}

@end
