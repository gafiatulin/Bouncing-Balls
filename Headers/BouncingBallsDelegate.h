//
//  BouncingBallsDelegate.h
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#ifndef __BouncingBalls__BouncingBallsDelegate__
#define __BouncingBalls__BouncingBallsDelegate__

#include "cinder/app/CinderView.h"
#include "cinder/Cinder.h"

#import <Cocoa/Cocoa.h>

#include "BouncingBallsApp.h"
#include "BallsGroup.h"

@interface BouncingBallsDelegate : NSObject <NSApplicationDelegate>
{
    IBOutlet NSWindow *window;
	IBOutlet CinderView	*cinderView;
	IBOutlet NSArrayController *arrayController;
    BouncingBallsApp *mApp;
}
@property IBOutlet NSWindow *window;
@property (nonatomic, strong) NSMutableArray *groups;
- (IBAction)loadOpenPanel:(NSMenuItem *)sender;
- (IBAction)loadSavePanel:(NSMenuItem *)sender;
- (IBAction)addGroupButtonClicked:(NSButton *)sender;
- (IBAction)loadGroupButtonClicked:(NSButton *)sender;

-(void) addBallWithID:(NSString *)ID;
-(void)addFiveBallsWithID:(NSString *)ID;
-(void)removeBallWithID:(NSString *)ID;
-(void)removeFiveBallsWithID:(NSString *)ID;
-(void)deleteGroupWithID:(NSString *)ID;
-(void)saveGroupWithID:(NSString *)ID;
@end

#endif /* defined(__BouncingBalls__BouncingBallsDelegate__) */