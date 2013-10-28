
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

#import "TriangulationViewController.h"
#import "triangulation/ntriangulation.h"

@interface TriangulationViewController () {
    UIViewController *_sub;
}
@property (weak, nonatomic) IBOutlet UITabBar *tabs;
@property (weak, nonatomic) IBOutlet UITabBarItem *gluingTab;
@property (weak, nonatomic) IBOutlet UITabBarItem *skeletonTab;
@property (weak, nonatomic) IBOutlet UITabBarItem *algebraTab;
@property (weak, nonatomic) IBOutlet UITabBarItem *snapPeaTab;
@end

@implementation TriangulationViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    // regina::NTriangulation* t = (regina::NTriangulation*)self.packet;

    _gluingTab.selectedImage = [UIImage imageNamed:@"Gluings-Bold"];
    _skeletonTab.selectedImage = [UIImage imageNamed:@"Skeleton-Bold"];
    _snapPeaTab.selectedImage = [UIImage imageNamed:@"SnapPea-Bold"];
    
    _tabs.selectedItem = _gluingTab;
    
    [_sub performSegueWithIdentifier:@"triDefault" sender:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // This is the embed segue.
    _sub = segue.destinationViewController;
}

@end
