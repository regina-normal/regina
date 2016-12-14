
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

#import "NewTri4Controller.h"
#import "PacketPicker.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "triangulation/example4.h"
#import "triangulation/dim3.h"
#import "triangulation/dim4.h"

@interface NewTri4Controller ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *types;
@property (weak, nonatomic) IBOutlet UIView *container;
@property (weak, nonatomic) NewPacketPageViewController *pages;
@end

@implementation NewTri4Controller

- (void)viewDidLoad
{
    self.pages = static_cast<NewPacketPageViewController*>(self.childViewControllers.lastObject);
    [self.pages fillWithPages:@[@"newTri4Empty", @"newTri4Example", @"newTri4Bundle", @"newTri4Isosig"]
                 pageSelector:self.types
                   defaultKey:@"NewTri4Page"];
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

#pragma mark - Empty page

@implementation NewTri4EmptyPage

- (regina::Packet*)create
{
    regina::Packet* ans = new regina::Triangulation<4>();
    ans->setLabel("4-D triangulation");
    return ans;
}

@end

#pragma mark - Example triangulation

typedef regina::Triangulation<4>* (*Tri4Creator)();

/**
 * Represents a single option in the examples picker.
 */
@interface Example4 : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) Tri4Creator creator;

+ (id)exampleWithName:(NSString*)name creator:(Tri4Creator)creator;
- (regina::Triangulation<4>*)create;

@end

@implementation Example4

+ (id)exampleWithName:(NSString *)name creator:(Tri4Creator)creator
{
    Example4* e = [[Example4 alloc] init];
    if (e) {
        e.name = name;
        e.creator = creator;
    }
    return e;
}

- (regina::Triangulation<4> *)create
{
    regina::Triangulation<4>* ans = (*self.creator)();
    ans->setLabel(self.name.UTF8String);
    return ans;
}

@end

#pragma mark - Example page

@interface NewTri4ExamplePage () <UIPickerViewDataSource, UIPickerViewDelegate> {
    NSArray* options;
}
@property (weak, nonatomic) IBOutlet UIPickerView *example;
@end

#define KEY_LAST_EXAMPLE @"NewTri4Example"

@implementation NewTri4ExamplePage

- (void)viewDidLoad
{
    options = @[[Example4 exampleWithName:@"4-sphere (minimal)" creator:&regina::Example<4>::fourSphere],
                [Example4 exampleWithName:@"4-sphere (simplex boundary)" creator:&regina::Example<4>::simplicialFourSphere],
                [Example4 exampleWithName:@"Cappell-Shaneson knot complement" creator:&regina::Example<4>::cappellShaneson],
                [Example4 exampleWithName:@"Product B³ × S¹" creator:&regina::Example<4>::ballBundle],
                [Example4 exampleWithName:@"Product S³ × S¹" creator:&regina::Example<4>::sphereBundle],
                [Example4 exampleWithName:@"ℝP⁴" creator:&regina::Example<4>::rp4],
                [Example4 exampleWithName:@"Twisted product B³ ×~ S¹" creator:&regina::Example<4>::twistedBallBundle],
                [Example4 exampleWithName:@"Twisted product S³ ×~ S¹" creator:&regina::Example<4>::twistedSphereBundle]];

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

#pragma mark - Bundle page

#define KEY_LAST_BUNDLE_TYPE @"NewTri4BundleType"

@interface NewTri4BundlePage ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *bundleType;
@property (weak, nonatomic) IBOutlet PacketPicker *from;
@end

@implementation NewTri4BundlePage

- (void)viewDidLoad
{
    NewTri4Controller* c = static_cast<NewTri4Controller*>(self.parentViewController.parentViewController);
    [self.from fill:c.spec.parent->root() type1:regina::PACKET_TRIANGULATION3 type2:regina::PACKET_SNAPPEATRIANGULATION allowNone:NO noneText:@"No 3-manifold triangulations in this document"];
    [self.bundleType setSelectedSegmentIndex:[[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_BUNDLE_TYPE]];
}

- (IBAction)bundleTypeChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setInteger:[self.bundleType selectedSegmentIndex] forKey:KEY_LAST_BUNDLE_TYPE];
}

- (regina::Packet*)create
{
    if ([self.from empty]) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No 3-Manifold Triangulations"
                                                        message:@"This document does not contain any 3-manifold triangulations, and so there is nothing for me to build a bundle from.  You can build a 4-D bundle by first adding a new 3-D triangulation to this document."
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

    regina::Triangulation<4>* ans;
    if (self.bundleType.selectedSegmentIndex == 0) {
        ans = regina::Example<4>::iBundle(*tri);

        if (tri->label().empty())
            ans->setLabel("I-bundle");
        else
            ans->setLabel(tri->label() + " × I");
    } else {
        ans = regina::Example<4>::s1Bundle(*tri);

        if (tri->label().empty())
            ans->setLabel("S¹-bundle");
        else
            ans->setLabel(tri->label() + " × S¹");
    }

    ans->intelligentSimplify();
    return ans;
}

@end

#pragma mark - Isosig page

@interface NewTri4IsosigPage ()
@property (weak, nonatomic) IBOutlet UITextField *isosig;
@end

@implementation NewTri4IsosigPage

- (IBAction)editingEnded:(id)sender {
    NewTri4Controller* c = static_cast<NewTri4Controller*>(self.parentViewController.parentViewController);
    [c create:sender];
}

- (regina::Packet *)create
{
    std::string sig = [self.isosig.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]].UTF8String;
    if (sig.empty()) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Empty Isomorphism Signature"
                              message:@"Please type an isomorphism signature into the box provided."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }
    
    regina::Triangulation<4>* t = regina::Triangulation<4>::fromIsoSig(sig);
    if (! t) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Invalid Isomorphism Signature"
                              message:nil
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }
    
    t->setLabel(sig);
    return t;
}

@end
