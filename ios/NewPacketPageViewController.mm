
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

#import "NewPacketPageViewController.h"
#import "PacketTreeController.h"

@interface NewPacketPageViewController() <UIPageViewControllerDataSource, UIPageViewControllerDelegate> {
    int currentPage;
    NSString* defaultKey;
}

@property (strong, nonatomic) NSMutableArray *pages;
@property (weak, nonatomic) UISegmentedControl* pageSelector;

@end

@implementation NewPacketPageViewController

- (void)fillWithPages:(NSArray *)storyboardIDs pageSelector:(UISegmentedControl *)s defaultKey:(NSString*)key
{
    self.pageSelector = s;
    
    id page;
    self.pages = [NSMutableArray array];
    for (id i in storyboardIDs) {
        page = [self.storyboard instantiateViewControllerWithIdentifier:i];
        if (! page)
            NSLog(@"No view controller for storyboard ID: %@", i);
        else if (! [[page class] conformsToProtocol:@protocol(PacketCreator)])
            NSLog(@"Storyboard ID %@ is not a PacketCreator.", i);
        else
            [self.pages addObject:page];
    }

    defaultKey = key;
    if (defaultKey) {
        currentPage = [[NSUserDefaults standardUserDefaults] integerForKey:defaultKey];
    } else
        currentPage = 0;
    
    self.dataSource = self;
    
    [self setViewControllers:[NSArray arrayWithObject:self.pages[currentPage]]
                   direction:UIPageViewControllerNavigationDirectionForward
                    animated:NO
                  completion:nil];
    
    self.pageSelector.selectedSegmentIndex = currentPage;
    
    self.delegate = self;
    
    [self.pageSelector addTarget:self action:@selector(updateCurrentPage) forControlEvents:UIControlEventValueChanged];
}

- (void)viewWillDisappear:(BOOL)animated
{
    if (defaultKey)
        [[NSUserDefaults standardUserDefaults] setInteger:currentPage forKey:defaultKey];
}

- (regina::Packet *)create
{
    id <PacketCreator> c = self.pages[currentPage];
    return [c create];
}

- (UIViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerAfterViewController:(UIViewController *)viewController
{
    // There are very few pages, and so indexOfObject will be fast enough.
    int index = [self.pages indexOfObject:viewController];

    if (index < self.pages.count - 1)
        return self.pages[index + 1];
    else
        return self.pages[0];
}

- (UIViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerBeforeViewController:(UIViewController *)viewController
{
    // There are very few pages, and so indexOfObject will be fast enough.
    int index = [self.pages indexOfObject:viewController];

    if (index > 0)
        return self.pages[index - 1];
    else
        return self.pages[self.pages.count - 1];
}

- (void)updateCurrentPage
{
    // Called when a new page is selected through the segmented control.
    int newPage = self.pageSelector.selectedSegmentIndex;
    if (newPage == currentPage)
        return;
    
    UIPageViewControllerNavigationDirection dir =
        (newPage < currentPage ?
         UIPageViewControllerNavigationDirectionReverse :
         UIPageViewControllerNavigationDirectionForward);

    // Note: the following call to setViewControllers *must* be done
    // without animation.
    //
    // Unfortunately, if we pass animated:YES then setViewControllers will
    // cache the view we left from, which means that if we "jumped" forward
    // more than one page then a gesture-based scroll backwards will take us
    // to the wrong place (specifically, the page we were at previously).
    //
    // However, animated:NO probably makes more sense from the user's point
    // of view also, since these pages are meant to be disjoint options:
    // if one page is visible then the others are irrelevant, and should not
    // appear to be linked together as some enormous collection of options.
    [self setViewControllers:[NSArray arrayWithObject:self.pages[newPage]]
                   direction:dir
                    animated:NO
                  completion:nil];

    currentPage = newPage;
}

- (void)pageViewController:(UIPageViewController *)pageViewController didFinishAnimating:(BOOL)finished previousViewControllers:(NSArray *)previousViewControllers transitionCompleted:(BOOL)completed
{
    // Called when a new page is selected through gesture-based page navigation.
    if (completed) {
        int newPage = [self.pages indexOfObject:self.viewControllers.firstObject];
        if (newPage == currentPage)
            return;
        
        currentPage = newPage;
        self.pageSelector.selectedSegmentIndex = newPage;
    }
}

@end

