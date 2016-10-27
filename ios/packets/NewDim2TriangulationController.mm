
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

#import "NewDim2TriangulationController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "dim2/dim2exampletriangulation.h"
#import "triangulation/dim2.h"

@interface NewDim2TriangulationController ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *types;
@property (weak, nonatomic) IBOutlet UIView *container;
@property (weak, nonatomic) NewPacketPageViewController *pages;
@end

@implementation NewDim2TriangulationController

- (void)viewDidLoad
{
    self.pages = static_cast<NewPacketPageViewController*>(self.childViewControllers.lastObject);
    [self.pages fillWithPages:@[@"newTri2Empty", @"newTri2Example", @"newTri2Surface", @"newTri2Isosig"]
                 pageSelector:self.types
                   defaultKey:@"NewDim2TriangulationPage"];
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

@implementation NewDim2TriangulationEmptyPage

- (regina::Packet*)create
{
    regina::Packet* ans = new regina::Triangulation<2>();
    ans->setLabel("2-D triangulation");
    return ans;
}

@end

#pragma mark - Example triangulation

typedef regina::Triangulation<2>* (*Dim2TriangulationCreator)();

/**
 * Represents a single option in the examples picker.
 */
@interface Dim2ExampleTriangulation : NSObject

@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) Dim2TriangulationCreator creator;

+ (id)exampleWithName:(NSString*)name creator:(Dim2TriangulationCreator)creator;
- (regina::Triangulation<2>*)create;

@end

@implementation Dim2ExampleTriangulation

+ (id)exampleWithName:(NSString *)name creator:(Dim2TriangulationCreator)creator
{
    Dim2ExampleTriangulation* e = [[Dim2ExampleTriangulation alloc] init];
    if (e) {
        e.name = name;
        e.creator = creator;
    }
    return e;
}

- (regina::Triangulation<2> *)create
{
    regina::Triangulation<2>* ans = (*self.creator)();
    ans->setLabel(self.name.UTF8String);
    return ans;
}

@end

#pragma mark - Example page

@interface NewDim2TriangulationExamplePage () <UIPickerViewDataSource, UIPickerViewDelegate> {
    NSArray* options;
}
@property (weak, nonatomic) IBOutlet UIPickerView *example;
@end

#define KEY_LAST_EXAMPLE @"NewDim2TriangulationExample"

@implementation NewDim2TriangulationExamplePage

- (void)viewDidLoad
{
    options = @[[Dim2ExampleTriangulation exampleWithName:@"Sphere (2 triangles)" creator:&regina::Dim2ExampleTriangulation::sphere],
                [Dim2ExampleTriangulation exampleWithName:@"Sphere (tetrahedron boundary)" creator:&regina::Dim2ExampleTriangulation::sphereTetrahedron],
                [Dim2ExampleTriangulation exampleWithName:@"Sphere (octahedron boundary)" creator:&regina::Dim2ExampleTriangulation::sphereOctahedron],
                [Dim2ExampleTriangulation exampleWithName:@"Disc" creator:&regina::Dim2ExampleTriangulation::disc],
                [Dim2ExampleTriangulation exampleWithName:@"Annulus" creator:&regina::Dim2ExampleTriangulation::annulus],
                [Dim2ExampleTriangulation exampleWithName:@"Möbius band" creator:&regina::Dim2ExampleTriangulation::mobius],
                [Dim2ExampleTriangulation exampleWithName:@"Torus" creator:&regina::Dim2ExampleTriangulation::torus],
                [Dim2ExampleTriangulation exampleWithName:@"Projective plane" creator:&regina::Dim2ExampleTriangulation::rp2],
                [Dim2ExampleTriangulation exampleWithName:@"Klein bottle" creator:&regina::Dim2ExampleTriangulation::kb]];
   
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

#pragma mark - Surface page

@interface NewDim2TriangulationSurfacePage ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *orbl;
@property (weak, nonatomic) IBOutlet UILabel *genusExpln;
@property (weak, nonatomic) IBOutlet UITextField *genus;
@property (weak, nonatomic) IBOutlet UITextField *punctures;
@end

@implementation NewDim2TriangulationSurfacePage

- (void)viewDidLoad
{
    // Set up the initial genus explanation message.
    [self orblChanged:nil];
}

- (int)checkGenus
{
    NSString* text = [self.genus.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if (text.length == 0) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Which genus?"
                              message:@"Please enter the genus of the surface into the box provided."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return -1;
    }
    int val = text.intValue;
    if (val < 0) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Invalid Genus"
                              message:@"The genus should be a non-negative integer."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return -1;
    }
    return val;
}

- (int)checkPunctures
{
    NSString* text = [self.punctures.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if (text.length == 0) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"How many punctures?"
                              message:@"Please enter the number of punctures into the box provided."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return -1;
    }
    int val = text.intValue;
    if (val < 0) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Invalid Number of Punctures"
                              message:@"The number of punctures should be a non-negative integer."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return -1;
    }
    return val;
}

- (IBAction)orblChanged:(id)sender {
    if (self.orbl.selectedSegmentIndex == 0)
        self.genusExpln.text = @"The orientable genus counts the number of handles.";
    else
        self.genusExpln.text = @"The non-orientable genus counts the number of crosscaps.";
}

- (IBAction)genusEditingEnded:(id)sender
{
    [self checkGenus];
}

- (IBAction)puncturesEditingEnded:(id)sender
{
    [self checkPunctures];
}

- (regina::Packet*)create
{
    int useGenus = [self checkGenus];
    if (useGenus < 0)
        return 0;
    
    int usePunctures = [self checkPunctures];
    if (usePunctures < 0)
        return 0;
    
    bool useOrbl = (self.orbl.selectedSegmentIndex == 0);
    if (useGenus == 0 && ! useOrbl) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@"Invalid Non-Orientable Genus"
                              message:@"A non-orientable surface must have strictly positive genus."
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil];
        [alert show];
        return 0;
    }

    regina::Packet* ans;
    if (useOrbl)
        ans = regina::Dim2ExampleTriangulation::orientable(useGenus, usePunctures);
    else
        ans = regina::Dim2ExampleTriangulation::nonOrientable(useGenus, usePunctures);
    return ans;
}

@end

#pragma mark - Isosig page

@interface NewDim2TriangulationIsosigPage ()
@property (weak, nonatomic) IBOutlet UITextField *isosig;
@end

@implementation NewDim2TriangulationIsosigPage

- (IBAction)editingEnded:(id)sender {
    NewDim2TriangulationController* c = static_cast<NewDim2TriangulationController*>(self.parentViewController.parentViewController);
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
    
    regina::Triangulation<2>* t = regina::Triangulation<2>::fromIsoSig(sig);
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
