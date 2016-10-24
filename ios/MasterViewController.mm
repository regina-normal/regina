
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

#import "DBChooser/DBChooser.h"
#import "MasterViewController.h"
#import "MBProgressHUD.h"
#import "ReginaDocument.h"
#import "ReginaHelper.h"

static UIColor* darkGoldenrod = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];
static UIColor* snapPeaGreen = [UIColor colorWithRed:0.0 green:0.5 blue:0.0 alpha:1.0];

#pragma mark - Document Spec

static NSDateFormatter* dateFormatter;

@interface DocumentSpec : NSObject

enum {
    DOCSPEC_TYPE_RGA,
    DOCSPEC_TYPE_TRI,
    DOCSPEC_TYPE_UNKNOWN
};

enum DocSource {
    DOCSOURCE_NATIVE,
    DOCSOURCE_DROPBOX
};

@property (strong, nonatomic, readonly) NSURL* url;
@property (strong, nonatomic, readonly) NSString* name;
@property (strong, nonatomic, readonly) NSAttributedString* displayName;
@property (strong, nonatomic, readonly) NSDate* lastModified;
@property (assign, nonatomic, readonly) int type;

- (id)initWithURL:(NSURL*)url;
+ (id)specWithURL:(NSURL*)url;

- (BOOL)refreshLastModified;
- (NSString*)lastModifiedText;

- (NSComparisonResult)compare:(DocumentSpec*)rhs;

@end

@implementation DocumentSpec

- (id)initWithURL:(NSURL *)url
{
    self = [super init];
    if (self) {
        _url = url;

        NSString* ext = url.pathExtension;
        if ([ext isEqualToString:@"rga"])
            _type = DOCSPEC_TYPE_RGA;
        else if ([ext isEqualToString:@"tri"])
            _type = DOCSPEC_TYPE_TRI;
        else
            _type = DOCSPEC_TYPE_UNKNOWN;

        // We don't use the NSURLLocalizedNameKey property, since sometimes this
        // strips the extension and sometimes it does not.
        _name = _url.lastPathComponent;
        switch (_type) {
            case DOCSPEC_TYPE_RGA:
                _name = [_name stringByDeletingPathExtension];
                _displayName = [[NSAttributedString alloc] initWithString:_name];
                break;
            case DOCSPEC_TYPE_TRI: {
                _displayName = [[NSAttributedString alloc] initWithString:_name attributes:@{NSForegroundColorAttributeName: snapPeaGreen}];
                break;
            }
            case DOCSPEC_TYPE_UNKNOWN:
                _displayName = [[NSAttributedString alloc] initWithString:_name attributes:@{NSForegroundColorAttributeName: darkGoldenrod}];
                break;
        }

        [self refreshLastModified];
    }
    return self;
}

+ (id)specWithURL:(NSURL *)url
{
    return [[DocumentSpec alloc] initWithURL:url];
}

- (BOOL)refreshLastModified
{
    NSError* err;
    NSDate* date;
    if ([_url getResourceValue:&date forKey:NSURLContentModificationDateKey error:&err]) {
        if (_lastModified && [_lastModified isEqualToDate:date])
            return NO;
        _lastModified = date;
        return YES;
    } else {
        NSLog(@"Error querying file modification time for %@: %@", _url, err.localizedDescription);
        _lastModified = nil;
        return NO;
    }
}

- (NSString *)lastModifiedText
{
    if (! dateFormatter) {
        dateFormatter = [[NSDateFormatter alloc] init];
        dateFormatter.dateStyle = NSDateFormatterMediumStyle;
        dateFormatter.timeStyle = NSDateFormatterShortStyle;
        dateFormatter.doesRelativeDateFormatting = YES;
    }
    return [dateFormatter stringFromDate:self.lastModified];
}

- (NSComparisonResult)compare:(DocumentSpec *)rhs
{
    return [self.name caseInsensitiveCompare:rhs.name];
}

@end

#pragma mark - Master view controller

@interface MasterViewController () <NSURLSessionDownloadDelegate> {
    MBProgressHUD* dropboxHUD;
    UIView* rootView;
}

/**
 * All local documents, sorted in the same order
 * as the visual table.
 *
 * Element type: (DocumentSpec*)
 */
@property (strong, nonatomic) NSMutableArray *docURLs;

/**
 * All local documents, indexed by URL.
 *
 * Key type: NSURL (note that keys are copied)
 * Value type: (DocumentSpec*)
 */
@property (strong, nonatomic) NSMutableDictionary* docsByName;

@end

@implementation MasterViewController

- (void)awakeFromNib
{
    self.preferredContentSize = CGSizeMake(320.0, 600.0);
    [super awakeFromNib];
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Meh.  Don't show the edit menu here - we don't get it in the packet tree anyway,
    // and better to have a consistent means of deleting things.
    // self.navigationItem.leftBarButtonItem = self.editButtonItem;

    self.docURLs = [NSMutableArray array];
    self.docsByName = [NSMutableDictionary dictionary];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self refreshDocURLs];
}

- (BOOL)openURL:(NSURL *)url preferredName:(NSURL *)name
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
        if ([url.scheme hasPrefix:@"db-"]) {
            // Looks like a Dropbox URL.
            return [[DBChooser defaultChooser] handleOpenURL:url];
        } else {
            // Download the contents of the URL, and save it to the local documents directory.
            NSURLSessionConfiguration* config = [NSURLSessionConfiguration defaultSessionConfiguration];
            NSURLSession* session = [NSURLSession sessionWithConfiguration:config delegate:self delegateQueue:nil];
            NSURLSessionDownloadTask* task = [session downloadTaskWithURL:url];
            
            rootView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
            dropboxHUD = [MBProgressHUD showHUDAddedTo:rootView animated:YES];
            dropboxHUD.mode = MBProgressHUDModeDeterminateHorizontalBar;
            dropboxHUD.progress = 0.0;
            dropboxHUD.label.text = @"Downloading...";

            [task resume];
            return YES;
        }
    }
    
    // This request may be coming from some other app, and this app may be
    // in the middle of something completely different.
    // Wind back anything that is happening in this app at present.
    // This will (amongst other things) have the effect of closing any document
    // that might currently be open.
    [self.navigationController popToRootViewControllerAnimated:NO];
    
    // Open the given document.
    ReginaDocument* doc = [ReginaDocument documentWithInboxURL:url preferredName:nil];
    if (doc) {
        [self performSegueWithIdentifier:@"openInbox" sender:doc];
        return YES;
    } else {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Could Not Save on Device"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return NO;
    }
}

- (BOOL)openURL:(NSURL *)url
{
    return [self openURL:url preferredName:nil];
}

- (void)newDocumentFrom:(DocSource)source
{
    switch (source) {
        case DOCSOURCE_NATIVE:
            // Create a new document.
            [self performSegueWithIdentifier:@"openNew" sender:self];
            break;
        case DOCSOURCE_DROPBOX:
            // Import from Dropbox.
            [[DBChooser defaultChooser] openChooserForLinkType:DBChooserLinkTypeDirect fromViewController:self completion:^(NSArray *results) {
                if ([results count])
                    [self openURL:[results.firstObject link]];
            }];
            break;
    }
}

- (IBAction)newDocument:(id)sender
{
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    [alert addAction:[UIAlertAction actionWithTitle:@"New document"
                                              style:UIAlertActionStyleDefault
                                            handler:^(UIAlertAction*) {
                                                        [self newDocumentFrom:DOCSOURCE_NATIVE];
                                                    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"Import from Dropbox"
                                              style:UIAlertActionStyleDefault
                                            handler:^(UIAlertAction*) {
                                                        [self newDocumentFrom:DOCSOURCE_DROPBOX];
                                                    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                              style:UIAlertActionStyleCancel
                                            handler:nil]];
    [alert setModalPresentationStyle:UIModalPresentationPopover];
    alert.popoverPresentationController.barButtonItem = self.navigationItem.rightBarButtonItem;
    [self presentViewController:alert animated:YES completion:nil];
}

- (ReginaDocument *)documentForIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
        return (indexPath.row == 0 ? [ReginaDocument documentWithExample:[Example intro]] : nil);
    else
        return [ReginaDocument documentWithURL:[self.docURLs[indexPath.row] url]];
}

- (void)refreshDocURLs
{
    // Flush out the ordered array of documents.
    // Any previous DocumentSpec objects will be preserved in docsByName, and reused if possible.
    [self.docURLs removeAllObjects];
    
    NSArray* contents = [[NSFileManager defaultManager] contentsOfDirectoryAtURL:[ReginaDocument docsDir]
                                                      includingPropertiesForKeys:@[NSURLContentModificationDateKey,
                                                                                   NSURLIsDirectoryKey]
                                                                         options:0
                                                                           error:nil];

    NSError* err;
    NSNumber* isDir;
    DocumentSpec* reuse;
    for (NSURL* url in [contents objectEnumerator]) {
        if ([url getResourceValue:&isDir forKey:NSURLIsDirectoryKey error:&err]) {
            if (isDir.boolValue)
                continue;
        } else {
            NSLog(@"Error scanning documents directory: %@", err.localizedDescription);
        }
        reuse = [self.docsByName objectForKey:url];
        if (reuse)
            [self.docURLs addObject:reuse];
        else
            [self.docURLs addObject:[DocumentSpec specWithURL:url]];
    }

    /*
    [self.docURLs sortUsingDescriptors:@[[NSSortDescriptor sortDescriptorWithKey:@"lastModified" ascending:NO],
                                         [NSSortDescriptor sortDescriptorWithKey:@"url" ascending:YES]]];
    */
    [self.docURLs sortUsingSelector:@selector(compare:)];

    // Now flush and refill docsByName.
    // This will destroy any old DocumentSpec objects that are no longer needed.
    [self.docsByName removeAllObjects];
    for (DocumentSpec* spec in [self.docURLs objectEnumerator]) {
        [self.docsByName setObject:spec forKey:spec.url];
    }

    [self.tableView reloadData];
}

- (void)refreshURL:(NSURL *)url
{
    DocumentSpec* spec = [self.docsByName objectForKey:url];
    if (spec) {
        if ([spec refreshLastModified]) {
            // There should not be enormously many documents; for now leave this as
            // a linear-time search for the corresponding table row.
            NSIndexPath* index = [NSIndexPath indexPathForRow:[self.docURLs indexOfObject:spec] inSection:1];
            [self.tableView reloadRowsAtIndexPaths:@[index] withRowAnimation:UITableViewRowAnimationAutomatic];
        }
    } else {
        // Suspicious.  We could not find this document in our list.
        // Reload everything just to be safe.
        NSLog(@"Warning: refreshURL could not locate URL: %@", url);
        [self refreshDocURLs];
    }
}

#pragma mark - Editable table view

- (BOOL)renameAllowed:(NSIndexPath *)path
{
    return path.section == 1;
}

- (NSString *)renameInit:(NSIndexPath *)path
{
    return static_cast<DocumentSpec*>(self.docURLs[path.row]).name;
}

- (void)renameDone:(NSIndexPath *)path result:(NSString *)result
{
    NSString* text = [result stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if (text.length == 0) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Empty Document Name"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    DocumentSpec* spec = self.docURLs[path.row];

    NSString* filename = [text stringByReplacingOccurrencesOfString:@"/" withString:@":"];
    if ([filename isEqualToString:spec.name])
        return;

    if (spec.type == DOC_NATIVE)
        filename = [filename stringByAppendingPathExtension:@"rga"];
    NSURL* docsDir = [ReginaDocument docsDir];
    NSURL* newURL = [docsDir URLByAppendingPathComponent:filename];
    if ([newURL isEqual:spec.url])
        return;

    // Some sanity checking to make sure there are no special characters doing unexpected things.
    NSURL* dir = [newURL URLByDeletingLastPathComponent];
    if (! [dir isEqual:docsDir]) {
        NSLog(@"Renamed URL not in documents directory: %@", newURL);
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Cannot Rename"
                              message:@"It is possible that your new document name contains special characters that I was not expecting."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    NSFileManager* manager = [NSFileManager defaultManager];
    if ([manager fileExistsAtPath:newURL.path]) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Name Already Taken"
                              message:@"Another document is already using this name."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    NSLog(@"Renaming: %@ -> %@", spec.url, newURL);
    if (! [manager moveItemAtURL:spec.url toURL:newURL error:nil]) {
        NSLog(@"Rename failed.");
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Could Not Rename"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    // All good.  Update the internal array and the visual table.
    [self.docURLs removeObjectAtIndex:path.row];
    [self.docsByName removeObjectForKey:spec.url];

    DocumentSpec* newSpec = [DocumentSpec specWithURL:newURL];
    NSUInteger newRow = [self.docURLs indexOfObject:newSpec
                                      inSortedRange:NSMakeRange(0, self.docURLs.count)
                                            options:NSBinarySearchingInsertionIndex
                                    usingComparator:^(DocumentSpec* x, DocumentSpec* y) {
                                        return [x compare:y];
                                    }];
    [self.docURLs insertObject:newSpec atIndex:newRow];
    [self.docsByName setObject:newSpec forKey:newURL];
    NSIndexPath* newPath = [NSIndexPath indexPathForRow:newRow inSection:1];
    if (newRow != path.row)
        [self.tableView moveRowAtIndexPath:path toIndexPath:newPath];
    [self.tableView reloadRowsAtIndexPaths:@[newPath] withRowAnimation:UITableViewRowAnimationAutomatic];
}

- (NSString *)deleteConfirmation:(NSIndexPath *)path
{
    return @"Delete document";
}

- (void)deleteAction
{
    NSURL* url = [self.docURLs[self.actionPath.row] url];
    NSLog(@"Deleting document: %@", url);
    if ([[NSFileManager defaultManager] removeItemAtURL:url error:nil]) {
        [self.docURLs removeObjectAtIndex:self.actionPath.row];
        [self.docsByName removeObjectForKey:url];
        [self.tableView deleteRowsAtIndexPaths:@[self.actionPath] withRowAnimation:UITableViewRowAnimationFade];
    } else {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Could Not Delete Document"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
    }
}

#pragma mark - URL Session

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
    NSLog(@"Downloaded to: %@", location);
 
    ReginaDocument* doc = [ReginaDocument documentWithInboxURL:location preferredName:downloadTask.currentRequest.URL];

    dispatch_async(dispatch_get_main_queue(), ^{
        [dropboxHUD hideAnimated:NO];
        dropboxHUD = nil;

        if (doc)
            [self performSegueWithIdentifier:@"openInbox" sender:doc];
        else {
            UIAlertView* alert = [[UIAlertView alloc]
                                  initWithTitle:@"Could Not Save on Device"
                                  message:nil
                                  delegate:nil
                                  cancelButtonTitle:@"Close"
                                  otherButtonTitles:nil];
            [alert show];
        }
    });
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    if (dropboxHUD && totalBytesExpectedToWrite > 0) {
        dispatch_async(dispatch_get_main_queue(), ^{
            dropboxHUD.progress = (float(totalBytesWritten)) / (float(totalBytesExpectedToWrite));
        });
    }
}

- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
    if (error) {
        NSLog(@"Download error.");
        dispatch_async(dispatch_get_main_queue(), ^{
            [dropboxHUD hideAnimated:NO];
            dropboxHUD = nil;

            UIAlertView* alert = [[UIAlertView alloc]
                                  initWithTitle:@"Could Not Download"
                                  message:nil
                                  delegate:nil
                                  cancelButtonTitle:@"Close"
                                  otherButtonTitles:nil];
            [alert show];
        });
    }
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didResumeAtOffset:(int64_t)fileOffset expectedTotalBytes:(int64_t)expectedTotalBytes
{
    // This should not be called, since we do not provide a way to resume downloads.
    NSLog(@"Unexpected NSURLSessionDownloadDelegate call to didResumeAtOffset.");
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
        DocumentSpec* spec = self.docURLs[indexPath.row];
        cell = [tableView dequeueReusableCellWithIdentifier:@"Document" forIndexPath:indexPath];
        cell.textLabel.attributedText = spec.displayName;
        cell.detailTextLabel.text = spec.lastModifiedText;
    }
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (! self.actionPath) && (indexPath.section == 1);
}

@end
