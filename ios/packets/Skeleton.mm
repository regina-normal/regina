
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

#import <UIKit/UIKit.h>
#import "Skeleton.h"
#import "TextHelper.h"
#import "TextPopover.h"

@implementation SkeletonHeaderCell
@end

@implementation SkeletonCell
@end

@interface SkeletonViewer ()
@end

@implementation SkeletonViewer

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.details addGestureRecognizer:r];
}

+ (NSAttributedString*)titleFromHeaderField:(UILabel*)field
{
    // Note: iOS uses \U00002028 as a line separator in labels.
    NSString* text = [NSString stringWithFormat:@"%@: ", [field.text stringByReplacingOccurrencesOfString:@"\U00002028" withString:@" "]];
    return [TextHelper markedString:text];
}

- (IBAction)longPress:(id)sender {
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    UIGestureRecognizerState state = press.state;
    
    CGPoint location = [press locationInView:self.details];
    NSIndexPath *indexPath = [self.details indexPathForRowAtPoint:location];
    
    // Don't take the header row.
    if ((! indexPath) || indexPath.row == 0)
        return;

    SkeletonCell *cell = static_cast<SkeletonCell*>([self.details cellForRowAtIndexPath:indexPath]);

    // Don't take an "empty triangulation" row.
    if (! cell.data0)
        return;
    
    if (state == UIGestureRecognizerStateBegan) {
        
        NSIndexPath *headerPath = [NSIndexPath indexPathForRow:0 inSection:0];
        SkeletonHeaderCell *header = static_cast<SkeletonHeaderCell*>([self.details cellForRowAtIndexPath:headerPath]);
        
        NSMutableAttributedString* detail = [[NSMutableAttributedString alloc] init];

        NSString* objectType;
        switch (header.tag) {
            case 0: objectType = @"Vertex"; break;
            case 1: objectType = @"Edge"; break;
            case 2: objectType = @"Triangle"; break;
            case 3: objectType = @"Tetrahedron"; break;
            case 4: objectType = @"Pentachoron"; break;
            case -1: objectType = @"Component"; break;
            case -2: objectType = @"Boundary component"; break;
            default: objectType = @"Unknown"; break;
        }

        [detail appendAttributedString:[TextHelper markedString:[NSString stringWithFormat:@"%@ #: ", objectType]]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%d\n", indexPath.row - 1]]];

        [detail appendAttributedString:[SkeletonViewer titleFromHeaderField:header.data0]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data0.text]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];

        if (header.data1) {
            [detail appendAttributedString:[SkeletonViewer titleFromHeaderField:header.data1]];
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data1.text]];
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
        }

        if (header.data2) {
            [detail appendAttributedString:[SkeletonViewer titleFromHeaderField:header.data2]];
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data2.text]];
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
        }

        if (header.data3) {
            [detail appendAttributedString:[SkeletonViewer titleFromHeaderField:header.data3]];
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data3.text]];
            [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
        }

        [detail addAttributes:@{NSFontAttributeName: cell.index.font} range:NSMakeRange(0, detail.length)];

        TextPopover* c = [self.storyboard instantiateViewControllerWithIdentifier:@"textPopover"];
        c.text = detail;
        UIPopoverController* detailPopover = [[UIPopoverController alloc] initWithContentViewController:c];
        [detailPopover presentPopoverFromRect:cell.frame
                                       inView:self.details
                     permittedArrowDirections:UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown
                                     animated:YES];
    }
}

@end
