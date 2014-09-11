
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

#import "NewSnapPeaController.h"
#import "PacketTreeController.h"
#import "snappea/nsnappeatriangulation.h"

@interface NewSnapPeaController ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *types;
@property (weak, nonatomic) IBOutlet UIView *container;
@property (weak, nonatomic) NewPacketPageViewController *pages;
@end

@implementation NewSnapPeaController

- (void)viewDidLoad
{
    self.pages = static_cast<NewPacketPageViewController*>(self.childViewControllers.lastObject);
    [self.pages fillWithPages:@[@"newSnapPeaConvert", @"newSnapPeaExample"]
                 pageSelector:self.types
                   defaultKey:@"NewSnapPeaPage"];
}

- (IBAction)create:(id)sender
{
    regina::NPacket* ans = [self.pages create];
    if (ans) {
        self.spec.parent->insertChildLast(ans);
        [self.spec.tree viewPacket:ans];
        [self dismissViewControllerAnimated:YES completion:nil];
    }
}

- (IBAction)cancel:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end

#pragma mark - Convert page

@interface NewSnapPeaConvertPage ()
@property (weak, nonatomic) IBOutlet UIPickerView *from;
@property (weak, nonatomic) IBOutlet UITextView *tip;
@end

@implementation NewSnapPeaConvertPage

- (regina::NPacket*)create
{
    // TODO.
    regina::NPacket* ans = new regina::NSnapPeaTriangulation();
    ans->setPacketLabel("SnapPea Triangulation");
    return ans;
}

@end

#pragma mark - Example triangulation

typedef regina::NSnapPeaTriangulation* (*SnapPeaCreator)();

/**
 * Represents a single option in the examples picker.
 */
@interface SnapPeaExampleTriangulation : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) SnapPeaCreator creator;

+ (id)exampleWithName:(NSString*)name creator:(SnapPeaCreator)creator;
- (regina::NSnapPeaTriangulation*)create;

@end

@implementation SnapPeaExampleTriangulation

+ (id)exampleWithName:(NSString *)name creator:(SnapPeaCreator)creator
{
    SnapPeaExampleTriangulation* e = [[SnapPeaExampleTriangulation alloc] init];
    if (e) {
        e.name = name;
        e.creator = creator;
    }
    return e;
}

- (regina::NSnapPeaTriangulation *)create
{
    // TODO
    regina::NSnapPeaTriangulation* ans = new regina::NSnapPeaTriangulation();
    ans->setPacketLabel(self.name.UTF8String);
    return ans;
}

@end

#pragma mark - Example page

@interface NewSnapPeaExamplePage () <UIPickerViewDataSource, UIPickerViewDelegate> {
    NSArray* options;
}
@property (weak, nonatomic) IBOutlet UIPickerView *example;
@end

#define KEY_LAST_EXAMPLE @"NewSnapPeaExample"

@implementation NewSnapPeaExamplePage

- (void)viewDidLoad
{
    options = @[[SnapPeaExampleTriangulation exampleWithName:@"Gieseking manifold" creator:nil],
                [SnapPeaExampleTriangulation exampleWithName:@"Figure eight knot complement" creator:nil],
                [SnapPeaExampleTriangulation exampleWithName:@"Whitehead link complement" creator:nil],
                [SnapPeaExampleTriangulation exampleWithName:@"Census manifold x101" creator:nil]];
   
    self.example.dataSource = self;
    self.example.delegate = self;
    
    [self.example selectRow:[[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_EXAMPLE] inComponent:0 animated:NO];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return options.count;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [options[row] name];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    [[NSUserDefaults standardUserDefaults] setInteger:[self.example selectedRowInComponent:0] forKey:KEY_LAST_EXAMPLE];
}

- (regina::NPacket *)create
{
    return [options[[self.example selectedRowInComponent:0]] create];
}

@end

