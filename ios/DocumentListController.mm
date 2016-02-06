
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

#import "DetailViewController.h"
#import "DocumentListController.h"
#import "PacketTreeController.h"

@interface DocumentListController () {
    DetailViewController *_detail;
    UIEdgeInsets _originalContentInsets;
    UIEdgeInsets _originalScrollIndicatorInsets;
}
@end

@implementation DocumentListController

- (void)awakeFromNib
{
    self.clearsSelectionOnViewWillAppear = NO;
    [super awakeFromNib];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    _detail.doc = nil;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _detail = (DetailViewController*)
        [[[[self splitViewController] viewControllers] lastObject] topViewController];
}

- (ReginaDocument *)documentForIndexPath:(NSIndexPath *)indexPath
{
    // Implemented by subclasses.
    return nil;
}

- (BOOL)shouldPerformSegueWithIdentifier:(NSString *)identifier sender:(id)sender
{
    return ! self.actionPath;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Note: the real work for loading a data file happens in a new thread.
    if ([[segue identifier] isEqualToString:@"openDocument"]) {
        // The user clicked on a table cell that represents a document.
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        ReginaDocument* doc = [self documentForIndexPath:indexPath];
        [[segue destinationViewController] openDocument:doc];
    } else if ([[segue identifier] isEqualToString:@"openInbox"]) {
        // Some other app sent a document to us.
        // The corresponding ReginaDocument will be contained in sender.
        [[segue destinationViewController] openDocument:sender];
    } else if ([[segue identifier] isEqualToString:@"openNew"]) {
        // We are creating a new document from scratch.
        [[segue destinationViewController] newDocument];
    }
}

@end
