//
//  BouncingBallsDelegate.mm
//  BouncingBalls
//
//  Created by Victor Gafiatulin on 29.09.13.
//
//

#import "BouncingBallsDelegate.h"

static NSArray *openFiles(NSArray *fileTypes)
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setFloatingPanel: YES];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:fileTypes];
    [panel setPrompt:@"Load"];
	NSInteger i = [panel runModal];
	if (i == NSOKButton)
		return [panel URLs];
    return nil;
}

static NSURL *saveFiles(NSArray *fileTypes)
{
    NSSavePanel *panel = [NSSavePanel savePanel];
    [panel setFloatingPanel: YES];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init] ;
    [formatter setDateFormat:@"yyyy_MM_dd_HH_MM_SS"];
    [panel setPrompt:@"Save"];
    [panel setCanCreateDirectories:NO];
    [panel setExtensionHidden:NO];
    [panel setFloatingPanel: YES];
    [panel setNameFieldStringValue:[formatter stringFromDate:[NSDate date]]];
    [panel setAllowedFileTypes:fileTypes];
	NSInteger i = [panel runModal];
	if (i == NSOKButton)
        return [panel URL];
    return nil;
}

@implementation BouncingBallsDelegate
@synthesize window;
@synthesize groups;

-(NSColor *)getRandomColor
{
    CGFloat hue = ( arc4random() % 256 / 256.0 );
    CGFloat saturation = ( arc4random() % 128 / 256.0 ) + 0.5;
    CGFloat brightness = ( arc4random() % 128 / 256.0 ) + 0.5;
    return [NSColor colorWithHue:hue saturation:saturation brightness:brightness alpha:1];
};

-(BallsGroup *)getGroupWithID:(NSString *) ID
{
    NSUInteger idx = [groups indexOfObjectPassingTest:^(id obj, NSUInteger idx, BOOL *stop)
                      {
                          BallsGroup *bg = (BallsGroup*)obj;
                          return [bg.uniqueID  isEqualToString:ID];
                      }];
    return groups[idx];
}

- (void)dealloc
{
    delete mApp;
}
	
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    mApp = new BouncingBallsApp;
    mApp->prepareLaunch();
    mApp->setupCinderView( cinderView, cinder::app::RendererGl::create());
    mApp->launch();
    groups = [[NSMutableArray alloc] init];
    BallsGroup *bg = [[BallsGroup alloc] init];
    bg.uniqueID = @"Default";
    bg.groupMaxRadius = 100;
    bg.groupMinRadius = 5;
    bg.groupColor = [self getRandomColor];
    bg.colorMayVary = YES;
    bg.ballsCounter = 5;
    [arrayController addObject:bg];
    for(auto i=0;i<5;i++)
        mApp->mBallController.addBallWithGroup([bg.uniqueID UTF8String],
                                               arc4random_uniform(bg.groupMaxRadius-bg.groupMinRadius)+bg.groupMinRadius,
                                               fmodl(1.0+[bg.groupColor hueComponent]+((bg.colorMayVary)?0.2/360.0:0.0)*((NSInteger)arc4random_uniform(360)-180),1.0),
                                               [bg.groupColor saturationComponent],
                                               [bg.groupColor brightnessComponent]);
}

- (IBAction)addGroupButtonClicked:(NSButton *)sender
{
    BallsGroup *bg = [[BallsGroup alloc] init];
    if ([groups count])
        bg.uniqueID = [@"Group " stringByAppendingString: [[NSString alloc] initWithFormat:@"%lu", (unsigned long)[groups count]]];
    else
        bg.uniqueID = @"Default";
    bg.groupMaxRadius = 100;
    bg.groupMinRadius = 5;
    bg.groupColor = [self getRandomColor];
    bg.colorMayVary = YES;
    bg.ballsCounter = 0;
    [arrayController addObject:bg];
}

- (IBAction)loadGroupButtonClicked:(NSButton *)sender
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^
    {
        NSURL *fileUrl = [openFiles([[NSArray alloc] initWithObjects:@"groupmodel", nil]) firstObject];
        if (fileUrl)
        {
            NSData *data = [NSData dataWithContentsOfURL:fileUrl];
            NSMutableArray *ballsStrings = [NSKeyedUnarchiver unarchiveObjectWithData:data];
            BallsGroup *bg = [[BallsGroup alloc] initWithString: ballsStrings[0]];
            [ballsStrings removeObjectAtIndex:0];
            for(NSString *str in ballsStrings)
            {
                NSArray *t = [str componentsSeparatedByString:@" "];
                mApp->mBallController.addBallWithGroup([bg.uniqueID UTF8String], [t[0] floatValue], [t[1] floatValue], [t[2] floatValue], [t[3] floatValue], [t[4] floatValue], [t[5] floatValue], [t[6] floatValue], [t[7] floatValue]);
            }
            [arrayController addObject:bg];
        }
    });
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

-(void)addBallWithID:(NSString *)ID
{
    BallsGroup *bg = [self getGroupWithID: ID];
    if (bg.groupMinRadius<=bg.groupMaxRadius & bg.groupMaxRadius<250 & bg.groupMinRadius>2)
    {
        mApp->mBallController.addBallWithGroup([bg.uniqueID UTF8String],
                                           arc4random_uniform(bg.groupMaxRadius-bg.groupMinRadius)+bg.groupMinRadius,
                                           fmodl(1.0+[bg.groupColor hueComponent]+((bg.colorMayVary)?0.2/360.0:0.0)*((NSInteger)arc4random_uniform(360)-180),1.0),
                                           [bg.groupColor saturationComponent],
                                           [bg.groupColor brightnessComponent]);
        bg.ballsCounter += 1;
    }
}

-(void)addFiveBallsWithID:(NSString *)ID
{
    for(auto i=0;i<5;i++)
        [self addBallWithID:ID];
}

-(void)removeBallWithID:(NSString *)ID
{
    BallsGroup *bg = [self getGroupWithID: ID];
    if (bg.ballsCounter>0)
    {
        mApp->mBallController.removeBallWithGroup([bg.uniqueID UTF8String]);
        bg.ballsCounter -= 1;
    }
}

-(void)removeFiveBallsWithID:(NSString *)ID
{
    for(auto i=0;i<5;i++)
        [self removeBallWithID:ID];
}

-(void)deleteGroupWithID:(NSString *)ID
{
    BallsGroup *bg = [self getGroupWithID: ID];
    int num = bg.ballsCounter;
    for(auto i=0;i<num;i++)
    {
        [self removeBallWithID:ID];
    }
    [arrayController removeObject:bg];
}

-(void)saveGroupWithID:(NSString *)ID
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^
    {
        std::vector<std::string> vector = mApp->mBallController.getBallsWithGroup([ID UTF8String]);;
        NSMutableArray *ballsStrings = [[NSMutableArray alloc] init];
        std::for_each(vector.begin(), vector.end(),[&ballsStrings](std::string str)
                      {
                          NSString *nsstr = [NSString stringWithUTF8String:str.c_str()];
                          [ballsStrings addObject:nsstr];
                      });
        if([ballsStrings count])
        {
            BallsGroup *bg = [self getGroupWithID: ID];
            [ballsStrings insertObject:[bg description] atIndex:0];
            NSData *data = [NSKeyedArchiver archivedDataWithRootObject:ballsStrings];
            [data writeToURL:saveFiles([[NSArray alloc] initWithObjects:@"groupmodel", nil]) atomically:YES];
        }
    });
}

- (IBAction)loadSavePanel:(NSMenuItem *)sender
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^
    {
        NSMutableArray *ballsGroupArray = [[NSMutableArray alloc] init];
        for (BallsGroup *bg in groups)
        {
            if(bg.ballsCounter>0)
            {
                std::vector<std::string> vector = mApp->mBallController.getBallsWithGroup([bg.uniqueID UTF8String]);;
                NSMutableArray *ballsStrings = [[NSMutableArray alloc] init];
                std::for_each(vector.begin(), vector.end(),[&ballsStrings](std::string str)
                              {
                                  NSString *nsstr = [NSString stringWithUTF8String:str.c_str()];
                                  [ballsStrings addObject:nsstr];
                              });
                [ballsStrings insertObject:[bg description] atIndex:0];
                [ballsGroupArray addObject:ballsStrings];
            }
        }
        NSData *data = [NSKeyedArchiver archivedDataWithRootObject:ballsGroupArray];
        [data writeToURL:saveFiles([[NSArray alloc] initWithObjects:@"model", nil]) atomically:YES];
    });
}

- (IBAction)loadOpenPanel:(NSMenuItem *)sender
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^
    {
        NSURL *fileUrl = [openFiles([[NSArray alloc] initWithObjects:@"model", nil]) firstObject];
        if (fileUrl)
        {
            NSData *data = [NSData dataWithContentsOfURL:fileUrl];
            NSMutableArray *ballsGroupArray = [[NSMutableArray alloc] init];
            ballsGroupArray = [NSKeyedUnarchiver unarchiveObjectWithData:data];
            for (NSMutableArray *ballsStrings in ballsGroupArray)
            {
                BallsGroup *bg = [[BallsGroup alloc] initWithString: ballsStrings[0]];
                [ballsStrings removeObjectAtIndex:0];
                for(NSString *str in ballsStrings)
                {
                    NSArray *t = [str componentsSeparatedByString:@" "];
                    mApp->mBallController.addBallWithGroup([bg.uniqueID UTF8String], [t[0] floatValue], [t[1] floatValue], [t[2] floatValue], [t[3] floatValue], [t[4] floatValue], [t[5] floatValue], [t[6] floatValue], [t[7] floatValue]);
                }
                [arrayController addObject:bg];
            }
        }
    });
}

@end
