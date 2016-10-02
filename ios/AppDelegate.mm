
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

#import "AppDelegate.h"
#import "DetailViewController.h"
#import "MasterViewController.h"
#import "ReginaHelper.h"
#import "file/globaldirs.h"

@interface AppDelegate ()
@property (strong, nonatomic) NSURL* launchedURL;
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    //
    // From the iOS App Programming Guide:
    // - Assume portrait orientation here.  If the device is in landscape
    //   then the system will rotate the views later, but before displaying them.
    // - This routine must be fast.  Long tasks should be run on a secondary thread.
    //
    [ReginaHelper initWithApp:self];

    // Have we been asked to open an URL (e.g., from an "Open with..." action)?
    self.launchedURL = [launchOptions objectForKey:UIApplicationLaunchOptionsURLKey];
    
    // Make sure that Regina knows where to find its internal data files.
    NSString* path = [[NSBundle mainBundle] resourcePath];
    const char* home = [path UTF8String];
    regina::GlobalDirs::setDirs(home, "", home);
    
    return NO;
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    // It seems that this is called only if an URL is passed whilst the application
    // is already running.  If the application was not running, then we need to collect
    // the URL from application:didFinishLaunchingWithOptions:.
    return [[ReginaHelper master] openURL:url];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if (self.launchedURL) {
        // Open the URL that was passed at launch time.
        [[ReginaHelper master] openURL:self.launchedURL];
        self.launchedURL = nil;
    }
}
							
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // We need to save data, and also remember what we were doing so that
    // we can restore state later.
    // This routine *must* return quickly; otherwise the app may simply be killed.
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    // AFAICT, UIDocument seems to be saving on background/termination automatically.
    // An explicit save seems to be unnecessary.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // We need to save data and state here.
    // This *must* be fast: it has a strict time limit of ~5s, and there is
    // no way to request more time.
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    // AFAICT, UIDocument seems to be saving on background/termination automatically.
    // An explicit save seems to be unnecessary.
}

@end
