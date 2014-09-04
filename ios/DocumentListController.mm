
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

#import "DetailViewController.h"
#import "DocumentListController.h"
#import "PacketTreeController.h"
#import "MBProgressHUD.h"

@interface DocumentListController () {
    DetailViewController *_detail;
}
@end

@implementation DocumentListController

- (void)awakeFromNib
{
    self.clearsSelectionOnViewWillAppear = NO;
    [super awakeFromNib];
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _detail = (DetailViewController*)
        [[[[self splitViewController] viewControllers] lastObject] topViewController];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([[segue identifier] isEqualToString:@"openExample"]) {
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        
        // We use an activity indicator since files could take some time to load.
        UIView* rootView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
        MBProgressHUD* hud = [MBProgressHUD showHUDAddedTo:rootView animated:YES];
        [hud setLabelText:@"Loading"];
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            // The real work: load the file.
            Example* e = [Example all][indexPath.row];
            [[segue destinationViewController] openExample:e];

            dispatch_async(dispatch_get_main_queue(), ^{
                PacketTreeController* c = [segue destinationViewController];
                [c setTitle:e.desc];
                [c refreshPackets];
                [MBProgressHUD hideHUDForView:rootView animated:YES];
            });
        });
        
        [_detail viewOpenFile];
    }
}

@end
