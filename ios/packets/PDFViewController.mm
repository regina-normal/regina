
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

#import "PDFViewController.h"
#import "TempFile.h"
#import "packet/npdf.h"

// TODO: Give the temporary PDF file a more meaningful filename.
// TODO: View the PDF natively, instead of relying on Quick Look.

@interface PDFViewController () <UIDocumentInteractionControllerDelegate>
@property (weak, nonatomic) IBOutlet UILabel *detail;
@property (weak, nonatomic) IBOutlet UILabel *size;
@property (weak, nonatomic) IBOutlet UIButton *previewButton;
@property (strong, nonatomic) UIBarButtonItem* shareButton;
@property (strong, nonatomic) UIDocumentInteractionController *interact;
@property (strong, nonatomic) TempFile *tempFile;
@end

@implementation PDFViewController

/**
 * Sets up the necessary internal structures for working with the PDF data,
 * if this has not been done already.
 *
 * @return \c true on success, or \c false on failure.
 */
- (BOOL)initInteraction
{
    if (! self.tempFile) {
        self.tempFile = [TempFile tempFileWithExtension:@"pdf"];
        NSLog(@"Temporary PDF filename: %@", self.tempFile.filename);
        
        if (! static_cast<regina::NPDF*>(self.packet)->savePDF([self.tempFile.filename UTF8String])) {
            NSLog(@"Could not write temporary PDF file: %@", self.tempFile.filename);
            self.tempFile = nil;
            return NO;
        }
    }
    
    if (! self.interact) {
        self.interact = [UIDocumentInteractionController interactionControllerWithURL:self.tempFile.url];
        self.interact.delegate = self;
    }
    
    return YES;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Load the contents of the packet.
    unsigned long s = static_cast<regina::NPDF*>(self.packet)->size();
    if (! s) {
        self.detail.text = @"Empty PDF document";
        self.size.text = @"";
        
        self.previewButton.enabled = NO;
        
        // We cannot disable the share button here, since it will not have been created yet.
        // We disable the share button later, at the point of its creation.
    } else {
        self.detail.text = @"PDF document";
        
        // Meh.  We will not special-case the text for s=1, since PDF documents should not be 1 byte anyway.
        if (s < 1000)
            self.size.text = [NSString stringWithFormat:@"%ld B", s];
        else if (s < 1000000)
            self.size.text = [NSString stringWithFormat:@"%ld kB", (unsigned long)(round(s * 1.0 / 1e3))];
        else if (s < 1000000000)
            self.size.text = [NSString stringWithFormat:@"%ld MB", (unsigned long)(round(s * 1.0 / 1e6))];
        else
            self.size.text = [NSString stringWithFormat:@"%ld GB", (unsigned long)(round(s * 1.0 / 1e9))];
    }
}

- (void)didMoveToParentViewController:(UIViewController *)parent
{
    if (parent) {
        // Offer a Share button so that we can view the PDF in some other app.
        self.shareButton = [[UIBarButtonItem alloc]
                                        initWithBarButtonSystemItem:UIBarButtonSystemItemAction
                                        target:self
                                        action:@selector(sharePDF:)];
        if (static_cast<regina::NPDF*>(self.packet)->isNull())
            self.shareButton.enabled = NO;
        
        self.detailViewController.navigationItem.rightBarButtonItem = self.shareButton;
    }
}

- (void)willMoveToParentViewController:(UIViewController *)parent
{
    if (! parent) {
        // Remove the Share button now that our view is going away.
        self.detailViewController.navigationItem.rightBarButtonItem = nil;
    }
}

- (UIViewController *) documentInteractionControllerViewControllerForPreview: (UIDocumentInteractionController *) controller {
    return self;
}

/**
 * Gives the user a full-screen quick preview of the PDF document.
 */
- (IBAction)previewPDF:(id)sender
{
    [self initInteraction];
    [self.interact presentPreviewAnimated:YES];
}

/**
 * Offers the user the opportunity to open the PDF document in some different app.
 */
- (IBAction)sharePDF:(id)sender
{
    [self initInteraction];
    [self.interact presentOptionsMenuFromBarButtonItem:self.shareButton animated:YES];
}

@end
