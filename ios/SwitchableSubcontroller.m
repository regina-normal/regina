
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

#import "SwitchableSubcontroller.h"

@interface SwitchableSubcontroller ()

@end

@implementation SwitchableSubcontroller

- (UIViewController *)subview
{
    return self.childViewControllers.lastObject;
}

- (void)addAutoLayoutConstraints:(UIView*)subView {
    NSDictionary *views = NSDictionaryOfVariableBindings(subView);
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[subView]|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:views]];
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[subView]|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:views]];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    UIViewController* to = segue.destinationViewController;
    
    // Switch off autoresizing now, so we can safely add autolayout constraints later.
    to.view.translatesAutoresizingMaskIntoConstraints = NO;

    if (self.childViewControllers.count > 0) {
        UIViewController* from = self.childViewControllers.lastObject;
        
        [from willMoveToParentViewController:nil];
        [self addChildViewController:to];
        [self transitionFromViewController:from
                          toViewController:to
                                  duration:0
                                   options:0
                                animations:nil
                                completion:^(BOOL finished) {
                                    [from removeFromParentViewController];
                                    [self addAutoLayoutConstraints:to.view];
                                    [to didMoveToParentViewController:self];
                                }];
    } else {
        [self addChildViewController:to];
        [self.view addSubview:to.view];
        [self addAutoLayoutConstraints:to.view];
        [to didMoveToParentViewController:self];
    }
}

@end
