
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

#import "MasterViewController.h"
#import "ReginaDocument.h"

// TODO: Support dropbox documents.
// TODO: Allow renaming documents.

// Action sheet tags;
enum {
    sheetNew,
    sheetDelete
};

@interface MasterViewController () <UIActionSheetDelegate> {
    UITableView* deleteTableView;
    NSIndexPath* deleteIndexPath;
}
@property (strong, nonatomic) NSMutableArray *docURLs;
@end

@implementation MasterViewController

- (void)awakeFromNib
{
    // Apple tells us that this is how to test for iOS6.x vs iOS 7:
    if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_6_1) {
        NSLog(@"Running in iOS 6.x");
        self.contentSizeForViewInPopover = CGSizeMake(320.0, 600.0);
    } else {
        NSLog(@"Running in iOS 7 or above");
        // This method crashes in iOS6 (unrecognised selector).
        self.preferredContentSize = CGSizeMake(320.0, 600.0);
    }
    [super awakeFromNib];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationItem.leftBarButtonItem = self.editButtonItem;
    
    UIBarButtonItem *addButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(newDocument:)];
    self.navigationItem.rightBarButtonItem = addButton;

    self.docURLs = [NSMutableArray array];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self refreshDocURLs];
}

- (BOOL)openURL:(NSURL *)url
{
    if (! url) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Missing URL"
                              message:@"I have been asked to open a data file, but no URL was given."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return NO;
    }

    if (! [url isFileURL]) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Not a local file"
                              message:@"At present, I can only open data files that are stored on this device.  "
                                "I cannot open network URLs (http, ftp, and so on)."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return NO;
    }
    
    // This request may be coming from some other app, and this app may be
    // in the middle of something completely different.
    // Wind back anything that is happening in this app at present.
    // This will (amongst other things) have the effect of closing any document
    // that might currently be open.
    [self.navigationController popToRootViewControllerAnimated:NO];
    
    // Open the given document.
    [self performSegueWithIdentifier:@"openInbox" sender:url];
    return YES;
}

- (void)newDocument:(id)sender
{
    UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                         destructiveButtonTitle:nil
                                              otherButtonTitles:@"New document", nil];
    sheet.tag = sheetNew;
    [sheet showFromBarButtonItem:self.navigationItem.rightBarButtonItem animated:YES];
}

- (ReginaDocument *)documentForIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
        return (indexPath.row == 0 ? [ReginaDocument documentWithExample:[Example intro]] : nil);
    else
        return [ReginaDocument documentWithURL:self.docURLs[indexPath.row]];
}

- (void)refreshDocURLs
{
    // TODO: Sort, timestamp, etc.
    [self.docURLs removeAllObjects];
    
    NSArray* contents = [[NSFileManager defaultManager] contentsOfDirectoryAtURL:[ReginaDocument docsDir] includingPropertiesForKeys:nil options:nil error:nil];
    for (NSURL* url in [contents objectEnumerator]) {
        [self.docURLs addObject:url];
    }
    
    [self.tableView reloadData];
}

#pragma mark - Table View

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0)
        return 2;
    else
        return self.docURLs.count;
}

- (NSString*)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if (section == 0)
        return @"Examples";
    else
        return @"Documents";
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell;
    
    if (indexPath.section == 0) {
        if (indexPath.row == 0) {
            cell = [tableView dequeueReusableCellWithIdentifier:@"Example" forIndexPath:indexPath];
            cell.textLabel.text = [Example intro].desc;
        } else {
            cell = [tableView dequeueReusableCellWithIdentifier:@"Census" forIndexPath:indexPath];
        }
    } else {
        cell = [tableView dequeueReusableCellWithIdentifier:@"Document" forIndexPath:indexPath];
        NSURL* url = self.docURLs[indexPath.row];
        // TODO: Strip the extension.
        cell.textLabel.text = url.lastPathComponent;
    }
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (indexPath.section == 1);
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return UITableViewCellEditingStyleDelete;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        deleteTableView = tableView;
        deleteIndexPath = indexPath;
        UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:@"Are you sure?" delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:@"Delete file" otherButtonTitles:nil];
        sheet.tag = sheetDelete;
        [sheet showInView:tableView];
    }
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (actionSheet.tag) {
        case sheetNew:
            if (buttonIndex == 0)
                [self performSegueWithIdentifier:@"openNew" sender:self];
            break;
        case sheetDelete:
            if (buttonIndex == actionSheet.destructiveButtonIndex) {
                NSURL* url = self.docURLs[deleteIndexPath.row];
                NSLog(@"Deleting document: %@", url);
                if ([[NSFileManager defaultManager] removeItemAtURL:url error:nil]) {
                    [self.docURLs removeObjectAtIndex:deleteIndexPath.row];
                    [deleteTableView deleteRowsAtIndexPaths:@[deleteIndexPath] withRowAnimation:UITableViewRowAnimationFade];
                } else {
                    UIAlertView* alert = [[UIAlertView alloc]
                                          initWithTitle:@"Could not delete document"
                                          message:nil
                                          delegate:nil
                                          cancelButtonTitle:@"Close"
                                          otherButtonTitles:nil];
                    [alert show];
                }
            }
            break;
    }
}

@end
