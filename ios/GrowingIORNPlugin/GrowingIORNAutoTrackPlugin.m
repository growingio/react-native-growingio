//
//  GrowingIORNAutoTrackPlugin.m
//  GrowingIORNPlugin
//
//  Created by apple on 2018/8/27.
//  Copyright © 2018年 GrowingIO. All rights reserved.
//

#import "GrowingIORNAutoTrackPlugin.h"

#define GrowingReactNativeAutoTrack @"GrowingReactNativeAutoTrack"

@implementation GrowingIORNAutoTrackPlugin

RCT_EXPORT_MODULE(GrowingIOModule);

RCT_EXPORT_METHOD(onPagePrepare:(NSString *)page)
{
    Class class = NSClassFromString(GrowingReactNativeAutoTrack);
    BOOL responds = [class respondsToSelector:@selector(onPagePrepare:)];
    
    if (!class || !responds) {
        return;
    }
    
    [class performSelector:@selector(onPagePrepare:) withObject:page];
}

RCT_EXPORT_METHOD(onPageShow:(NSString *)page)
{
    Class class = NSClassFromString(GrowingReactNativeAutoTrack);
    BOOL responds = [class respondsToSelector:@selector(onPageShow:)];
    
    if (!class || !responds) {
        return;
    }
    
    [class performSelector:@selector(onPageShow:) withObject:page];
        
}

RCT_EXPORT_METHOD(prepareView:(nonnull NSNumber *)tag isClickable:(BOOL)isClickable parameters:(NSDictionary *)parameters)
{
    Class class = NSClassFromString(GrowingReactNativeAutoTrack);
    BOOL responds = [class respondsToSelector:@selector(prepareView:parameters:)];
    
    if (!class || !responds) {
        return;
    }
    if (tag == nil) {
        return;
    }
    NSDictionary *tagClickDict = @{@"tag":tag, @"isClickable":[NSNumber numberWithBool:isClickable]};
    [class performSelector:@selector(prepareView:parameters:) withObject:tagClickDict withObject:parameters];
}

RCT_EXPORT_METHOD(onClick:(nonnull NSNumber *)tag)
{
    Class class = NSClassFromString(GrowingReactNativeAutoTrack);
    BOOL responds = [class respondsToSelector:@selector(onClick:)];
    
    if (!class || !responds) {
        return;
    }
    [class performSelector:@selector(onClick:) withObject:tag];
        
}

- (dispatch_queue_t)methodQueue
{
    return dispatch_get_main_queue();
}

+ (BOOL)requiresMainQueueSetup {
    return YES;
}

@end
