
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

#import "PacketDetailController.h"
#import "PacketManagerIOS.h"
#import "packet/npacket.h"
#import "packet/packettype.h"

@interface PacketDetailController () {
    NSString* _menuTitle;
}
@property (weak, nonatomic) UIViewController *sub;
@end

@implementation PacketDetailController

#pragma mark - Managing the detail item

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    [self viewWelcome];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWelcome {
    _packet = nil;
    
    [self navigationItem].title = @"";
    [_sub performSegueWithIdentifier:@"embedWelcome" sender:nil];

    _menuTitle = @"Documents";
    UIBarButtonItem* left = self.navigationItem.leftBarButtonItem;
    if (left)
        left.title = _menuTitle;
}

- (void)viewOpenFile {
    _packet = nil;
    
    [self navigationItem].title = @"";
    [_sub performSegueWithIdentifier:@"embedEmpty" sender:nil];
    
    _menuTitle = @"Packets";
    UIBarButtonItem* left = self.navigationItem.leftBarButtonItem;
    if (left)
        left.title = _menuTitle;
}

- (void)viewPacket:(regina::NPacket *)p {
    _packet = p;
    
    if (p == nil) {
        [self navigationItem].title = @"";
        [_sub performSegueWithIdentifier:@"embedEmpty" sender:nil];
    } else {
        [self navigationItem].title = [NSString stringWithUTF8String:p->getPacketLabel().c_str()];
        [_sub performSegueWithIdentifier:[PacketManagerIOS segueFor:p] sender:self];
    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([segue.identifier isEqualToString:@"embed"]) {
        _sub = segue.destinationViewController;
    }
    
    return;
    if (self.childViewControllers.count > 0) {
        NSLog(@"Swap");
        UIViewController* from = [self.childViewControllers objectAtIndex:0];
        UIViewController* to = segue.destinationViewController;

        [from willMoveToParentViewController:nil];
        [self addChildViewController:to];
        [self transitionFromViewController:from
                          toViewController:to
                                  duration:0
                                   options:0
                                animations:nil
                                completion:^(BOOL finished) {
                                    [from removeFromParentViewController];
                                    [to didMoveToParentViewController:self];
                                }]; // TODO: Necessary?
    } else {
        NSLog(@"Add");
        UIViewController* to = segue.destinationViewController;

        [self addChildViewController:to];
        
        [self.view addSubview:to.view];
        [to didMoveToParentViewController:self];
    }
}

#pragma mark - Split view

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
    barButtonItem.title = _menuTitle;
    [self.navigationItem setLeftBarButtonItem:barButtonItem animated:YES];
}

- (void)splitViewController:(UISplitViewController *)splitController willShowViewController:(UIViewController *)viewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem
{
    // Called when the view is shown again in the split view, invalidating the button and popover controller.
    [self.navigationItem setLeftBarButtonItem:nil animated:YES];
}

@end
