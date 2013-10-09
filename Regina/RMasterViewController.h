//
//  RMasterViewController.h
//  Regina
//
//  Created by Ben Burton on 9/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RDetailViewController;

@interface RMasterViewController : UITableViewController

@property (strong, nonatomic) RDetailViewController *detailViewController;

@end
