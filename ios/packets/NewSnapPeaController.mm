
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

#import "NewSnapPeaController.h"
#import "PacketPicker.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "snappea/examplesnappea.h"
#import "snappea/snappeatriangulation.h"

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
    regina::Packet* ans = [self.pages create];
    if (ans) {
        self.spec.parent->insertChildLast(ans);
        [self.spec created:ans];
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
@property (weak, nonatomic) IBOutlet PacketPicker *from;
@property (weak, nonatomic) IBOutlet UITextView *tip;
@end

@implementation NewSnapPeaConvertPage

- (void)viewDidLoad
{
    NewSnapPeaController* c = static_cast<NewSnapPeaController*>(self.parentViewController.parentViewController);
    [self.from fill:c.spec.parent->root() type:regina::PACKET_TRIANGULATION3 allowNone:NO noneText:@"No Regina triangulations in this document"];
}

- (regina::Packet*)create
{
    if ([self.from empty]) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Triangulations to Convert"
                                                        message:@"This document does not contain any triangulations in Regina's native format, and so there is nothing for me to convert.  You can build one by adding a new 3-D triangulation to this document."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }

    regina::Triangulation<3>* tri = static_cast<regina::Triangulation<3>*>([self.from selectedPacket]);
    if (! tri) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Triangulation Selected"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }

    if (tri->isEmpty()) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Empty Triangulation"
                                                        message:@"The selected triangulation does not contain any tetrahedra, and so I cannot convert into SnapPea format."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }
    if ((! tri->isConnected()) || tri->hasBoundaryTriangles() || (! tri->isValid()) || (! tri->isStandard())) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cannot Convert Triangulation"
                                                        message:@"I cannot convert this triangulation into SnapPea format.  SnapPea can only work with triangulations that are (i) valid and connected; (ii) have no boundary triangles; and (iii) where every ideal vertex has a torus or Klein bottle link."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }

    regina::SnapPeaTriangulation* ans = new regina::SnapPeaTriangulation(*tri);
    if (ans->isNull()) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could Not Convert to SnapPea"
                                                        message:@"I was unable to convert this into a SnapPea triangulation.  Regina works with more general triangulations than SnapPea does, and not every Regina triangulation can be converted into SnapPea's format."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
    }

    ans->setLabel(tri->label());
    return ans;
}

@end

#pragma mark - Example triangulation

typedef regina::SnapPeaTriangulation* (*SnapPeaCreator)();

/**
 * Represents a single option in the examples picker.
 */
@interface SnapPeaExampleTriangulation : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) SnapPeaCreator creator;

+ (id)exampleWithName:(NSString*)name creator:(SnapPeaCreator)creator;
- (regina::SnapPeaTriangulation*)create;

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

- (regina::SnapPeaTriangulation *)create
{
    regina::SnapPeaTriangulation* ans = (*self.creator)();
    ans->setLabel(self.name.UTF8String);
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
    options = @[[SnapPeaExampleTriangulation exampleWithName:@"Gieseking manifold" creator:&regina::ExampleSnapPea::gieseking],
                [SnapPeaExampleTriangulation exampleWithName:@"Figure eight knot complement" creator:&regina::ExampleSnapPea::figureEight],
                [SnapPeaExampleTriangulation exampleWithName:@"Trefoil complement" creator:&regina::ExampleSnapPea::trefoil],
                [SnapPeaExampleTriangulation exampleWithName:@"Whitehead link complement" creator:&regina::ExampleSnapPea::whiteheadLink],
                [SnapPeaExampleTriangulation exampleWithName:@"Census manifold x101" creator:&regina::ExampleSnapPea::x101]];
   
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

- (regina::Packet *)create
{
    return [options[[self.example selectedRowInComponent:0]] create];
}

@end

