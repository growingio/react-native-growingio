//
//  GrowingCoreKit.h
//  GrowingCoreKit
//
//  Created by GrowingIO on 2018/5/14.
//  Copyright © 2018年 GrowingIO. All rights reserved.
//

#import <UIKit/UIKit.h>

#ifndef __cplusplus
@import Foundation;
@import CoreTelephony;
@import SystemConfiguration;
@import Security;
@import CFNetwork;
@import CoreLocation;
#endif

typedef NS_ENUM(NSInteger, GrowingAspectMode)
{
    // 默认 类似KVO的机制进行数据采集
    // 目前已知对Aspects以及部分不确定的手写swizzling有冲突
    // 依据不同调用顺序 可能出现函数不被调用或者崩溃的问题
    // 另外如果使用 object_getClass或者swfit的dynamicType属性会得到一个KVO的子类
    // 如果用于取得XIB或者其他资源 可能会失效
    GrowingAspectModeSubClass           ,
    
    // 测试阶段 高兼容性 性能比GrowingAspectTypeSubClass略低 但是比RAC和Aspects快8-10倍左右
    GrowingAspectModeDynamicSwizzling   ,
};

@interface Growing : NSObject

// SDK版本号
+ (NSString*)sdkVersion;

// 如果您的AppDelegate中 实现了其中一个或者多个方法 请以在已实现的函数中 调用handleUrl
//- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(id)annotation
//- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
//- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString*, id> *)options
// 如果以上所有函数都未实现 则请实现 - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(id)annotation 方法并调用handleUrl
+ (BOOL)handleUrl:(NSURL*)url;

// 请在applicationDidFinishLaunching中调用此函数初始化

// TODO: 需要告诉客户如何获取 appId
// TODO: 要不要把 accountId 改成 projectId？
// TODO: 这里的任何改动，都需要改 help 文档和集成文档

// 如果需要采样 设置一个采样值   0.01即1% 0.001即1‰  最多支持小数点后5位
+ (void)startWithAccountId:(NSString *)accountId withAppId:(NSString *)appId withSampling:(CGFloat)sampling;

+ (void)startWithAccountId:(NSString *)accountId withSampling:(CGFloat)sampling;

// 默认采样100%
+ (void)startWithAccountId:(NSString *)accountId withAppId:(NSString *)appId;

+ (void)startWithAccountId:(NSString *)accountId;

// 命令行输出调试日志
+ (void)setEnableLog:(BOOL)enableLog;
+ (BOOL)getEnableLog;

// 以下函数设置后会覆盖原有设置
// 并且只会在第一次安装后调用 以保证同一设备的设备ID相同
// 请在方法 startWithAccountId 之前调用
// 使用自定义的ID 自定义ID长度不可大于64 否则会被抛弃 NSUUID的UUIDString长度为36
// Example:
//  1. setDeviceIDModeToCustomBlock:^NSString*{ return [[[UIDevice currentDevice] identifierForVendor] UUIDString]; }
+ (void)setDeviceIDModeToCustomBlock:(NSString*(^)(void))customBlock;

/**
 deeplink广告落地页参数回调设置

 @param handler: deeplink广告落地页参数回调, params 为解析正确时反回调的参数, error 为解析错误时返回的参数.
                 handler 默认为空, 客户需要手动设置.
 */
+ (void)registerDeeplinkHandler:(void(^)(NSDictionary *params, NSError *error))handler;
/**
 Universallink广告落地页参数回调设置
 
 @param handler: Universallink广告落地页参数回调, params 为解析正确时反回调的参数, error 为解析错误时返回的参数.
 handler 默认为空, 客户需要手动设置.
 */
+ (void)registerUniversallinkHandler:(void(^)(NSDictionary *params, NSError *error))handler;

// 该函数请在main函数第一行调用 APP启动后 将不允许修改采集模式
+ (void)setAspectMode:(GrowingAspectMode)aspectMode;
+ (GrowingAspectMode)getAspectMode;

// 是否允许发送基本性能诊断信息，默认为开
+ (void)setEnableDiagnose:(BOOL)enable;

// 全局不发送统计信息
+ (void)disable;

// 设置发送数据的时间间隔（单位为秒）
+ (void)setFlushInterval:(NSTimeInterval)interval;
+ (NSTimeInterval)getFlushInterval;

// 设置每天使用数据网络（2G、3G、4G）上传的数据量的上限（单位是 KB）
+ (void)setDailyDataLimit:(NSUInteger)numberOfKiloByte;
+ (NSUInteger)getDailyDataLimit;

// 设置数据收集平台服务器地址
+ (void)setTrackerHost:(NSString *)host;

// 设置设备报活服务器地址
+ (void)setReportHost:(NSString *)host;

// 设置数据查看平台服务器地址
+ (void)setDataHost:(NSString*)host;

// 设置数据后台服务器地址
+ (void)setGtaHost:(NSString*)host;

// 设置数据后台服务器地址
+ (void)setWsHost:(NSString*)host;

// 设置zone信息
+ (void)setZone:(NSString *)zone;

// 设置 GDPR 生效
+ (void)disableDataCollect;
// 设置 GDPR 失效
+ (void)enableDataCollect;
// 获取当前设备id
+ (NSString *)getDeviceId;
// 获取当前uid
+ (NSString *)getVisitUserId;
// 获取当前访问id
+ (NSString *)getSessionId;


/// !!!: V2.0埋点相关API，请在主线程里调用.
/**
 设置登录用户ID
 
 @param userId 登陆用户ID, ID为正常英文数字组合的字符串, 长度<=1000, 请不要含有 "'|\*&$@/', 等特殊字符
 ！！！不允许传空或者nil, 如有此操作请调用clearUserId函数
 */
+ (void)setUserId:(NSString *)userId;
/**
 清除登录用户ID
 */
+ (void)clearUserId;
/**
 转化变量
 
 @param variable : 转化变量, 变量不能为nil
 */
+ (void)setEvar:(NSDictionary<NSString *, NSObject *> *)variable;
/**
 转化变量 API
 
 @param key : 变量名称, 变量名称不能为nil或者""
 @param stringValue : 字符串变量, 变量不为nil或者""
 */
+ (void)setEvarWithKey:(NSString *)key andStringValue:(NSString *)stringValue;
/**
 转化变量
 
 @param key : 变量名称, 变量名称不能为nil或者""
 @param numberValue : 数值类型变量, 变量不能为nil
 */
+ (void)setEvarWithKey:(NSString *)key andNumberValue:(NSNumber *)numberValue;

/**
 用户变量
 
 @param variable : 用户变量, 变量不能为nil
 */
+ (void)setPeopleVariable:(NSDictionary<NSString *, NSObject *> *)variable;
/**
 用户变量
 
 @param key : 变量名称, 变量名称不能为nil或者""
 @param stringValue : 字符串变量, 变量不能为nil或者""
 */
+ (void)setPeopleVariableWithKey:(NSString *)key andStringValue:(NSString *)stringValue;
/**
 用户变量
 
 @param key : 变量名称, 变量名称不能为nil或者""
 @param numberValue : 数值类型变量, 变量不能为nil
 */
+ (void)setPeopleVariableWithKey:(NSString *)key andNumberValue:(NSNumber *)numberValue;

/**
 发送事件 API, number默认为1
 
 @param eventId : 事件Id, Id为正常英文数字组合的字符串, 长度<=1000, 请不要含有 "'|\*&$@/', 等特殊字符
 */
+ (void)track:(NSString *)eventId;

/**
 发送事件 API
 
 @param eventId : 事件Id, Id为正常英文数字组合的字符串, 长度<=1000, 请不要含有 "'|\*&$@/', 等特殊字符
 @param number : 数值类型变量, number为正的整数或者浮点数
 */
+ (void)track:(NSString *)eventId withNumber:(NSNumber *)number;

/**
 发送事件 API
 
 @param eventId : 事件Id, Id为正常英文数字组合的字符串, 长度<=1000, 请不要含有 "'|\*&$@/', 等特殊字符
 @param number : 数值类型变量, number为正的整数或者浮点数
 @param variable : 事件变量, 变量不能为nil
 */
+ (void)track:(NSString *)eventId withNumber:(NSNumber *)number andVariable:(NSDictionary<NSString *, id> *)variable;

/**
 发送事件 API
 
 @param eventId 事件Id, Id为正常英文数字组合的字符串, 长度<=1000, 请不要含有 "'|\*&$@/', 等特殊字符
 @param variable : 事件变量, 变量不能为nil
 */
+ (void)track:(NSString *)eventId withVariable:(NSDictionary<NSString *, id> *)variable;

/**
 访问用户变量
 
 @param variable : 访问用户变量, 不能为nil
 */
+ (void)setVisitor:(NSDictionary<NSString *, NSObject *> *)variable;

@end
