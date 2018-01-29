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
        [Growing track:eventId withVariable:eventLevelVariable];
    }];
}

RCT_EXPORT_METHOD(trackWithNumber:(NSString *)eventId number:(double)number eventLevelVariable:(NSDictionary *)eventLevelVariable)
{
    [self dispatchInMainThread:^{
        [Growing track:eventId withNumber:[NSNumber numberWithDouble:number] andVariable:eventLevelVariable];
    }];
}

RCT_EXPORT_METHOD(page:(NSString *)page)
{
    NSString *pageTime = GROWGetTimestamp();
    [self dispatchInMainThread:^{
        [Growing trackPageWithPageName:page pageTime:pageTime];
    }];
}

RCT_EXPORT_METHOD(setPageVariable:(NSString *)page pageLevelVariables:(NSDictionary *)pageLevelVariables)
{
    [self dispatchInMainThread:^{
        [Growing setPageVariable:pageLevelVariables
                          toPage:page];
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

- (void)dispatchInMainThread:(void (^)(void))block
{
  if ([NSThread isMainThread]) {
    block();
  } else {
    dispatch_async(dispatch_get_main_queue(), block);
  }
}

@end
