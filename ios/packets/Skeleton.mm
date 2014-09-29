
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

#import <UIKit/UIKit.h>
#import "Skeleton.h"
#import "TextHelper.h"
#import "TextPopover.h"

// TODO: Long press views full (overflowing) contents of cell.
// TODO: Augh.  In ios7 (not ios8), skeleton tables run under tabs after switching between tabs
// TODO: Vertical space in ios7

@implementation SkeletonHeaderCell
@end

@implementation SkeletonCell
@end

@implementation SkeletonViewer

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.details addGestureRecognizer:r];
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
        UITableViewCell *header = [self.details cellForRowAtIndexPath:headerPath];
        
        // TODO: Fix text.
        NSMutableAttributedString* detail = [[NSMutableAttributedString alloc] init];
        [detail appendAttributedString:[TextHelper markedString:@"Index: "]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.index.text]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
        [detail appendAttributedString:[TextHelper markedString:@"Type: "]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data0.text]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
        [detail appendAttributedString:[TextHelper markedString:@"Size: "]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data1.text]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
        [detail appendAttributedString:[TextHelper markedString:@"Pieces: "]];
        [detail appendAttributedString:[[NSAttributedString alloc] initWithString:cell.data2.text]];
        [detail addAttributes:@{NSFontAttributeName: cell.index.font} range:NSMakeRange(0, detail.length)];
        
        TextPopover* c = [self.storyboard instantiateViewControllerWithIdentifier:@"textPopover"];
        c.text = detail;
        UIPopoverController* popover = [[UIPopoverController alloc] initWithContentViewController:c];
        [popover presentPopoverFromRect:cell.frame
                                 inView:self.details
               permittedArrowDirections:UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown
                               animated:YES];
    }
}

@end
