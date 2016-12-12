
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

#import "NewTri3Controller.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "triangulation/example3.h"
#import "triangulation/dim3.h"

@interface NewTri3Controller ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *types;
@property (weak, nonatomic) IBOutlet UIView *container;
@property (weak, nonatomic) NewPacketPageViewController *pages;
@end

@implementation NewTri3Controller

- (void)viewDidLoad
{
    self.pages = static_cast<NewPacketPageViewController*>(self.childViewControllers.lastObject);
    [self.pages fillWithPages:@[@"newTri3Empty", @"newTri3Example", @"newTri3Isosig"]
                 pageSelector:self.types
                   defaultKey:@"NewTri3Page"];
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

@implementation NewTri3EmptyPage

- (regina::Packet*)create
{
    regina::Packet* ans = new regina::Triangulation<3>();
    ans->setLabel("3-D triangulation");
    return ans;
}

@end

#pragma mark - Example triangulation

typedef regina::Triangulation<3>* (*Tri3Creator)();

/**
 * Represents a single option in the examples picker.
 */
@interface Example3 : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) Tri3Creator creator;

+ (id)exampleWithName:(NSString*)name creator:(Tri3Creator)creator;
- (regina::Triangulation<3>*)create;

@end

@implementation Example3

+ (id)exampleWithName:(NSString *)name creator:(Tri3Creator)creator
{
    Example3* e = [[Example3 alloc] init];
    if (e) {
        e.name = name;
        e.creator = creator;
    }
    return e;
}

- (regina::Triangulation<3> *)create
{
    regina::Triangulation<3>* ans = (*self.creator)();
    ans->setLabel(self.name.UTF8String);
    return ans;
}

@end

#pragma mark - Example page

@interface NewTri3ExamplePage () <UIPickerViewDataSource, UIPickerViewDelegate> {
    NSArray* options;
}
@property (weak, nonatomic) IBOutlet UIPickerView *example;
@end

#define KEY_LAST_EXAMPLE @"NewTri3Example"

@implementation NewTri3ExamplePage

- (void)viewDidLoad
{
    options = @[[Example3 exampleWithName:@"3-sphere (minimal)" creator:&regina::Example<3>::threeSphere],
                [Example3 exampleWithName:@"3-sphere (dual to Bing's house)" creator:&regina::Example<3>::bingsHouse],
                [Example3 exampleWithName:@"3-sphere (simplex boundary)" creator:&regina::Example<3>::simplicialSphere],
                [Example3 exampleWithName:@"3-sphere (600-cell)" creator:&regina::Example<3>::sphere600],
                [Example3 exampleWithName:@"Connected sum ℝP³ # ℝP³" creator:&regina::Example<3>::rp3rp3],
                [Example3 exampleWithName:@"Figure eight knot complement" creator:&regina::Example<3>::figureEight],
                [Example3 exampleWithName:@"Gieseking manifold" creator:&regina::Example<3>::gieseking],
                [Example3 exampleWithName:@"Lens space L(8,3)" creator:[](){ return regina::Example<3>::lens(8, 3); }],
                [Example3 exampleWithName:@"Poincaré homology sphere" creator:&regina::Example<3>::poincareHomologySphere],
                [Example3 exampleWithName:@"Product ℝP² × S¹" creator:&regina::Example<3>::rp2xs1],
                [Example3 exampleWithName:@"Product S² × S¹" creator:&regina::Example<3>::s2xs1],
                [Example3 exampleWithName:@"ℝP³" creator:[](){ return regina::Example<3>::lens(2, 1); }],
                [Example3 exampleWithName:@"Solid Klein bottle (B² ×~ S¹)" creator:&regina::Example<3>::solidKleinBottle],
                [Example3 exampleWithName:@"Solid Torus (B² × S¹)" creator:&regina::Example<3>::ballBundle],
                [Example3 exampleWithName:@"Trefoil knot complement" creator:&regina::Example<3>::trefoil],
                [Example3 exampleWithName:@"Twisted product S² ×~ S¹" creator:&regina::Example<3>::twistedSphereBundle],
                [Example3 exampleWithName:@"Weeks manifold" creator:&regina::Example<3>::weeks],
                [Example3 exampleWithName:@"Weber-Seifert dodecahedral space" creator:&regina::Example<3>::weberSeifert],
                [Example3 exampleWithName:@"Whitehead link complement" creator:&regina::Example<3>::whiteheadLink]];
    
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

#pragma mark - Isosig page

@interface NewTri3IsosigPage ()
@property (weak, nonatomic) IBOutlet UITextField *isosig;
@end

@implementation NewTri3IsosigPage

- (IBAction)editingEnded:(id)sender {
    NewTri3Controller* c = static_cast<NewTri3Controller*>(self.parentViewController.parentViewController);
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
    
    regina::Triangulation<3>* t = regina::Triangulation<3>::fromIsoSig(sig);
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
