
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#import "LinkViewController.h"
#import "LinkCodes.h"
#import "link/link.h"

#define KEY_LINK_CODE_TYPE @"LinkCodeType"

@interface LinkCodes () <UITextViewDelegate>
@property (weak, nonatomic) IBOutlet UILabel *header;

@property (weak, nonatomic) IBOutlet UITextView *code;
@property (weak, nonatomic) IBOutlet UISegmentedControl *codeType;

@property (assign, nonatomic) regina::Link* packet;
@end

@implementation LinkCodes

- (void)viewDidLoad
{
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.view addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    self.codeType.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LINK_CODE_TYPE];
    
    self.packet = static_cast<regina::Link*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    [self reloadPacket];
}

- (void)reloadPacket
{
    [static_cast<LinkViewController*>(self.parentViewController) updateHeader:self.header];

    NSString* ans = nil;
    switch (self.codeType.selectedSegmentIndex) {
        case 0:
            if (self.packet->countComponents() != 1) {
                self.code.text = @"Gauss codes are currently only available for knots.";
                return;
            }

            ans = [NSString stringWithFormat:@"Oriented:\n%s\n\nClassical:\n%s\n",
                   self.packet->orientedGauss().c_str(),
                   self.packet->gauss().c_str()];
            break;

        case 1:
            if (self.packet->countComponents() != 1) {
                self.code.text = @"Dowker-Thistlethwaite notation is currently only available for knots.";
                return;
            }

            {
                // Restrict scope of local variables.
                std::string alpha = self.packet->dt(true);
                std::string numer = self.packet->dt(false);
                if (alpha.empty())
                    ans = @(numer.c_str());
                else
                    ans = [NSString stringWithFormat:
                           @"%s\n\n%s\n", alpha.c_str(), numer.c_str()];
            }
            break;

        case 2:
            if (self.packet->countComponents() != 1) {
                self.code.text = @"Knot signatures are currently only available for knots.";
                return;
            }

            ans = @(self.packet->knotSig().c_str());
            break;

        case 3:
            ans = @(self.packet->jenkins().c_str());
            break;
    }
    
    ans = [ans stringByReplacingOccurrencesOfString:@"-" withString:@"−"]; /* minus sign */
    ans = [ans stringByReplacingOccurrencesOfString:@" " withString:@"\u2002"]; /* enspace */
    self.code.text = ans;
}

- (IBAction)codeTypeChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setInteger:self.codeType.selectedSegmentIndex forKey:KEY_LINK_CODE_TYPE];
    [self reloadPacket];
}

- (IBAction)longPress:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    if (press.state == UIGestureRecognizerStateBegan) {
        CGPoint location = [press locationInView:self.view];
        if (! CGRectContainsPoint(self.code.frame, location))
            return;
        
        [self becomeFirstResponder];
        
        UIMenuController *copyMenu = [UIMenuController sharedMenuController];
        [copyMenu setTargetRect:CGRectMake(location.x, location.y, 0, 0) inView:self.view];
        copyMenu.arrowDirection = UIMenuControllerArrowDefault;
        [copyMenu setMenuVisible:YES animated:YES];
    }
}

#pragma mark - UIResponder

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    if (action == @selector(copy:)) {
        // Don't allow us to copy a Gauss code that is not being displayed.
        return ! (self.codeType.selectedSegmentIndex != 3 && self.packet->countComponents() != 1);
    } else
        return [super canPerformAction:action withSender:sender];
}

- (void)copy:(id)sender
{
    switch (self.codeType.selectedSegmentIndex) {
        case 0:
            if (self.packet->countComponents() == 1)
                [[UIPasteboard generalPasteboard] setString:[NSString stringWithFormat:
                                                             @"Oriented:\n%s\n\nClassical:\n%s\n",
                                                             self.packet->orientedGauss().c_str(),
                                                             self.packet->gauss().c_str()]];
            return;
        case 1:
            if (self.packet->countComponents() == 1) {
                NSString* ans = nil;

                std::string alpha = self.packet->dt(true);
                std::string numer = self.packet->dt(false);
                if (alpha.empty())
                    ans = @(numer.c_str());
                else
                    ans = [NSString stringWithFormat:
                           @"%s\n\n%s\n", alpha.c_str(), numer.c_str()];

                [[UIPasteboard generalPasteboard] setString:ans];
            }
            return;
        case 2:
            if (self.packet->countComponents() == 1)
                [[UIPasteboard generalPasteboard] setString:@(self.packet->knotSig().c_str())];
            return;
        case 3:
            [[UIPasteboard generalPasteboard] setString:@(self.packet->jenkins().c_str())];
            return;
    }
}

@end
