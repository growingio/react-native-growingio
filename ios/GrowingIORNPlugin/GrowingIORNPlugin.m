//
//  GrowingIORNPlugin.m
//  React
//
//  Created by apple on 2017/9/12.
//  Copyright © 2017年 Facebook. All rights reserved.
//

#import "GrowingIORNPlugin.h"
#import "Growing.h"

#define k_DEBUG  @"debug"
#define k_ZONE   @"zone"

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

RCT_EXPORT_METHOD(init:(NSString *)accountId strOptionJson:(NSString *)strOptionJson callback:(RCTResponseSenderBlock)callback)
{
    NSString *callbackInfo = nil;
    if (accountId.length == 0) {
        callbackInfo = @"Argument error, The argument accountId can not be empty";
    } else {
        NSDictionary *optionDict = nil;
        if (strOptionJson.length) {
            optionDict = [self getJsonObjectFromeString:strOptionJson];
            if (![optionDict isKindOfClass:[NSDictionary class]]) {
                callbackInfo = @"Argument error, The strOptionJson can not Convert to json";
                if (callback) {
                    callback(@[callbackInfo]);
                    return;
                }
            }
        }
        [Growing startWithAccountId:accountId];
        [self dispatchInMainThread:^{
            [Growing trackPageWithPageName:@"RNInitPage" pageTime:GROWGetTimestamp()];
        }];
        NSArray *messageArray = [self setOptions:optionDict];
        if (((NSNumber *)messageArray[0]).intValue == 0) {
            callbackInfo = messageArray[1];
        } else {
            callbackInfo = CALLBACK_SUCCESS;
        }
    }
    
    if (callback) {
        callback(@[callbackInfo]);
    }
}

RCT_EXPORT_METHOD(setUserId:(NSString *)userId callback:(RCTResponseSenderBlock)callback)
{
  NSString *callbackInfo = nil;
  [self dispatchInMainThread: ^{
    static NSString *lastUserId = nil;
    
    [Growing setUserId:userId];
    
    if (lastUserId.length == 0) {
      [Growing trackPageWithPageName:@"RNInitUserIdPage" pageTime:GROWGetTimestamp()];
    } else if (lastUserId.length != 0) {
      [Growing trackPageWithPageName:@"RNChangeUserIdPage" pageTime:GROWGetTimestamp()];
    } else if (lastUserId.length != 0 && userId.length == 0) {
      [Growing trackPageWithPageName:@"RNCleanUserIdPage" pageTime:GROWGetTimestamp()];
    }
    
    lastUserId = userId;
  }];
  callbackInfo = CALLBACK_SUCCESS;
  
  if (callback) {
    callback(@[callbackInfo]);
  }
}

RCT_EXPORT_METHOD(cleanUserId:(RCTResponseSenderBlock)callback)
{
  [self dispatchInMainThread: ^{
    [Growing clearUserId];
  }];
  
  NSString *callbackInfo = CALLBACK_SUCCESS;
  if (callback) {
    callback(@[callbackInfo]);
  }
}

RCT_EXPORT_METHOD(track:(NSString *)eventName number:(NSString *)number strEventJson:(NSString *)strEventJson callback:(RCTResponseSenderBlock)callback)
{
  NSString *callbackInfo = nil;
  if (eventName.length == 0) {
    callbackInfo = @"Argument error, The argument eventName can not be empty";
  } else {
    NSNumber *argumentNumber = nil;
    if (number.length) {
      argumentNumber = [NSNumber numberWithDouble:number.doubleValue];
    }
    
    NSDictionary *jsonDict = nil;
    if (strEventJson.length) {
      jsonDict = [self getJsonObjectFromeString:strEventJson];
      if (![jsonDict isKindOfClass:[NSDictionary class]]) {
        callbackInfo = @"Argument error, The strEventJson can not Convert to json";
        if (callback) {
          callback(@[callbackInfo]);
          return;
        }
      }
    }
    [self dispatchInMainThread:^{
      [Growing track:eventName withNumber:argumentNumber andVariable:jsonDict];
    }];
    callbackInfo = CALLBACK_SUCCESS;
  }
  
  if (callback) {
    callback(@[callbackInfo]);
  }
}


RCT_EXPORT_METHOD(trackPage:(NSString *)pageName pageTime:(NSString *)pageTime callback:(RCTResponseSenderBlock)callback)
{
  NSString *callbackInfo = nil;
  if (pageName.length == 0 || pageTime.length == 0) {
    callbackInfo = @"Argument error, The pageName or pageTime can not be empty";
  } else {
    [self dispatchInMainThread:^{
      [Growing trackPageWithPageName:pageName pageTime:pageTime];
    }];
    callbackInfo = CALLBACK_SUCCESS;
  }
  
  if (callback) {
    callback(@[callbackInfo]);
  }
}

RCT_EXPORT_METHOD(setAppVariable:(NSString *)strVariableJson callback:(RCTResponseSenderBlock)callback)
{
  [self performPluginSelName:@"setAppVariable" variable:strVariableJson callback:callback];
}

RCT_EXPORT_METHOD(setEvarVariable:(NSString *)strVariableJson callback:(RCTResponseSenderBlock)callback)
{
  [self performPluginSelName:@"setEvarVariable" variable:strVariableJson callback:callback];
}

RCT_EXPORT_METHOD(setPeopleVariable:(NSString *)strVariableJson callback:(RCTResponseSenderBlock)callback)
{
  [self performPluginSelName:@"setPeopleVariable" variable:strVariableJson callback:callback];
}

- (void)performPluginSelName:(NSString *)selName variable:(NSString *)strVariableJson callback:(RCTResponseSenderBlock)callback
{
  NSString *callbackInfo = nil;
  id jsonObj = [self getJsonObjectFromeString:strVariableJson];
  if ([jsonObj isKindOfClass:[NSDictionary class]]) {
    if (((NSDictionary *)jsonObj).count) {
      [self dispatchInMainThread: ^{
        if ([selName isEqualToString:@"setAppVariable"]) {
          [Growing setAppVariable:jsonObj];
        } else if ([selName isEqualToString:@"setEvarVariable"]) {
          [Growing setEvar:jsonObj];
        } else if ([selName isEqualToString:@"setPeopleVariable"]) {
          [Growing setPeopleVariable:jsonObj];
        }
      }];
      callbackInfo = CALLBACK_SUCCESS;
    } else {
      callbackInfo = @"Argument error, The json can not be empty";
    }
  } else {
    callbackInfo = @"Argument error, The strVariableJson can not Convert to json";
  }
  
  if (callback) {
    callback(@[callbackInfo]);
  }
}

- (NSArray *)setOptions:(NSDictionary *)options
{
    for (NSString *optionKey in options.allKeys) {
        if ([optionKey isEqualToString:k_DEBUG]) {
            NSNumber *debug = [options objectForKey:k_DEBUG];
            if (debug.intValue >= 1) {
                [Growing setEnableLog:YES];
            } else {
                [Growing setEnableLog:NO];
            }
        } else if([optionKey isEqualToString:k_ZONE]) {
            id zone = [options objectForKey:k_ZONE];
            if ([zone isKindOfClass:[NSString class]]) {
                [Growing setZone:zone];
            } else {
                NSArray *array = @[@0, @"Argument error, The argument zone must be string type"];
                return array;
            }
        }
    }
    return @[@1];
}

- (void)dispatchInMainThread:(void (^)(void))block
{
  if ([NSThread isMainThread]) {
    block();
  } else {
    dispatch_async(dispatch_get_main_queue(), block);
  }
}

- (id)getJsonObjectFromeString:(NSString *)string
{
  NSData *data = [string dataUsingEncoding:NSUTF8StringEncoding];
  id jsonObj = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];

  return jsonObj;
}

@end


