//
//  GrowingIORNPlugin.m
//  React
//
//  Created by apple on 2017/9/12.
//  Copyright © 2017年 Facebook. All rights reserved.
//

#import "GrowingIORNPlugin.h"
#import "Growing.h"

NS_INLINE NSString *GROWGetTimestampFromTimeInterval(NSTimeInterval timeInterval) {
  return [NSNumber numberWithUnsignedLongLong:timeInterval * 1000.0].stringValue;
}

NS_INLINE NSString *GROWGetTimestamp() {
  return GROWGetTimestampFromTimeInterval([[NSDate date] timeIntervalSince1970]);
}

#define CALLBACK_SUCCESS @"200"

@implementation GrowingIORNPlugin

RCT_EXPORT_MODULE(GrowingIO);

- (NSDictionary *)constantsToExport
{
    return @{@"CALLBACK_SUCCESS": CALLBACK_SUCCESS};
}

RCT_EXPORT_METHOD(track:(NSString *)eventId eventLevelVariable:(NSDictionary *)eventLevelVariable)
{
    [self dispatchInMainThread:^{
        if (eventLevelVariable.count != 0) {
            [Growing track:eventId withVariable:eventLevelVariable];
        } else {
            [Growing track:eventId];
        }
    }];
}

RCT_EXPORT_METHOD(trackWithNumber:(NSString *)eventId number:(double)number eventLevelVariable:(NSDictionary *)eventLevelVariable)
{
    [self dispatchInMainThread:^{
        if (eventLevelVariable.count != 0) {
            [Growing track:eventId withNumber:[NSNumber numberWithDouble:number] andVariable:eventLevelVariable];
        } else {
            [Growing track:eventId withNumber:[NSNumber numberWithDouble:number]];
        }
    }];
}

RCT_EXPORT_METHOD(setEvar:(NSDictionary *)conversionVariables)
{
    [self dispatchInMainThread:^{
        [Growing setEvar:conversionVariables];
    }];
}

RCT_EXPORT_METHOD(setPeopleVariable:(NSDictionary *)peopleVariables)
{
    [self dispatchInMainThread:^{
        [Growing setPeopleVariable:peopleVariables];
    }];
}

RCT_EXPORT_METHOD(setUserId:(NSString *)userId)
{
  [self dispatchInMainThread: ^{
    [Growing setUserId:userId];
  }];
}

RCT_EXPORT_METHOD(clearUserId)
{
  [self dispatchInMainThread: ^{
    [Growing clearUserId];
  }];
}

RCT_EXPORT_METHOD(setVisitor:(NSDictionary *)variable)
{
    [self dispatchInMainThread:^{
        [Growing setVisitor:variable];
    }];
}

RCT_EXPORT_METHOD(onPagePrepare:(NSString *)page)
{
    Class class = NSClassFromString(@"GrowingReactNativeTrack");
    if (!class) {
        return;
    }
    [self dispatchInMainThread: ^{
        [class performSelector:@selector(onPagePrepare:) withObject:page];
        
    }];
}

RCT_EXPORT_METHOD(onPageShow:(NSString *)page)
{
    Class class = NSClassFromString(@"GrowingReactNativeTrack");
    if (!class) {
        return;
    }
    [self dispatchInMainThread: ^{
        [class performSelector:@selector(onPageShow:) withObject:page];

    }];
}

- (void)dispatchInMainThread:(void (^)(void))block
{
  if ([NSThread isMainThread]) {
    block();
  } else {
    dispatch_async(dispatch_get_main_queue(), block);
  }
}

@end
