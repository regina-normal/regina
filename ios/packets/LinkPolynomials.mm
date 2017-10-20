
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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
#import "LinkPolynomials.h"
#import "ReginaHelper.h"
#import "link/link.h"

#define MAX_LINK_AUTO_POLYNOMIALS 2 // TODO

@interface LinkPolynomials () <UITextFieldDelegate> {
    UILabel* copyFrom;
}
@property (weak, nonatomic) IBOutlet UILabel *header;

@property (weak, nonatomic) IBOutlet UILabel *jones;
@property (weak, nonatomic) IBOutlet UILabel *homfly;
@property (weak, nonatomic) IBOutlet UILabel *bracket;
@property (weak, nonatomic) IBOutlet UIButton *calculateJones;
@property (weak, nonatomic) IBOutlet UIButton *calculateHomfly;
@property (weak, nonatomic) IBOutlet UIButton *calculateBracket;

@property (assign, nonatomic) regina::Link* packet;
@end

@implementation LinkPolynomials

- (void)viewDidLoad
{
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.view addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = static_cast<regina::Link*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    [self reloadPacket];
}

- (IBAction)longPress:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    if (press.state == UIGestureRecognizerStateBegan) {
        /* TODO
        copyFrom = nil;
        CGPoint location = [press locationInView:self.view];
        if (CGRectContainsPoint(self.isosig.frame, location))
            copyFrom = self.isosig;
        else if (CGRectContainsPoint(self.standard.frame, location))
            copyFrom = self.standard;
        if (! copyFrom)
            return;
        
        [self becomeFirstResponder];
        
        UIMenuController *copyMenu = [UIMenuController sharedMenuController];
        CGRect textBounds = [copyFrom textRectForBounds:copyFrom.bounds limitedToNumberOfLines:copyFrom.numberOfLines];
        [copyMenu setTargetRect:textBounds inView:copyFrom];
        copyMenu.arrowDirection = UIMenuControllerArrowDefault;
        [copyMenu setMenuVisible:YES animated:YES];
        */
    }
}

- (void)reloadPacket
{
    [static_cast<LinkViewController*>(self.parentViewController) updateHeader:self.header];

    if (self.packet->knowsJones() || self.packet->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        self.calculateJones.hidden = YES;
        self.jones.text = @(self.packet->jones().str(regina::Link::jonesVar).c_str());
    } else {
        self.jones.text = @" ";
        self.calculateJones.hidden = NO;
        self.calculateJones.enabled = YES;
    }

    if (self.packet->knowsHomfly() || self.packet->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        self.calculateHomfly.hidden = YES;
        self.homfly.text = @(self.packet->homfly().str(regina::Link::homflyVarX, regina::Link::homflyVarY).c_str());
    } else {
        self.homfly.text = @" ";
        self.calculateHomfly.hidden = NO;
        self.calculateHomfly.enabled = YES;
    }

    if (self.packet->knowsBracket() || self.packet->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        self.calculateBracket.hidden = YES;
        self.bracket.text = @(self.packet->bracket().str("A").c_str());
    } else {
        self.bracket.text = @" ";
        self.calculateBracket.hidden = NO;
        self.calculateBracket.enabled = YES;
    }
}

- (IBAction)computeJones:(id)sender {
    self.calculateJones.enabled = NO;
    self.calculateHomfly.enabled = NO;
    self.calculateBracket.enabled = NO;
    [ReginaHelper runWithHUD:@"Calculating…"
                        code:^{
                            self.packet->jones();
                        }
                     cleanup:^{
                         [self reloadPacket];
                     }];
}

- (IBAction)computeHomfly:(id)sender {
    self.calculateHomfly.enabled = NO;
    self.calculateJones.enabled = NO;
    self.calculateBracket.enabled = NO;
    [ReginaHelper runWithHUD:@"Calculating…"
                        code:^{
                            self.packet->homfly();
                        }
                     cleanup:^{
                         [self reloadPacket];
                     }];
}

- (IBAction)computeBracket:(id)sender {
    self.calculateBracket.enabled = NO;
    self.calculateJones.enabled = NO;
    self.calculateHomfly.enabled = NO;
    [ReginaHelper runWithHUD:@"Calculating…"
                        code:^{
                            self.packet->bracket();
                        }
                     cleanup:^{
                         [self reloadPacket];
                     }];
}

#pragma mark - UIResponder

- (BOOL)canBecomeFirstResponder
{
    return (copyFrom != nil);
}

- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    if (action == @selector(copy:) && copyFrom)
        return YES;
    else
        return [super canPerformAction:action withSender:sender];
}

- (void)copy:(id)sender
{
    // TODO
    if (copyFrom)
        [[UIPasteboard generalPasteboard] setString:copyFrom.text];
}

@end
