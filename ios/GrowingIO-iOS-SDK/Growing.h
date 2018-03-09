//
//  growing.h
//  Growing
//
//  Created by GrowinIO on 5/3/15.
//  Copyright (c) 2015 GrowingIO. All rights reserved.
//


#import <UIKit/UIKit.h>

#ifndef __cplusplus
@import Foundation;
@import WebKit;
@import CoreTelephony;
@import SystemConfiguration;
@import Foundation;
@import Security;
@import CFNetwork;
@import CoreLocation;
@import Security;
#endif

typedef NS_ENUM(NSInteger, GrowingAspectMode)
{
    // 默认 类似KVO的机制进行数据采集
    // 目前已知对RAC的rac_signalForSelector和Aspects以及部分不确定的手写swizzling有冲突
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
+ (void)setDeviceIDModeToCustomBlock:(NSString*(^)())customBlock;

// 该函数请在main函数第一行调用 APP启动后 将不允许修改采集模式
+ (void)setAspectMode:(GrowingAspectMode)aspectMode;
+ (GrowingAspectMode)getAspectMode;

// 是否允许发送基本性能诊断信息，默认为开
+ (void)setEnableDiagnose:(BOOL)enable;

// 全局_不_发送任何统计信息
+ (void)disable;
// 设置为 NO，可以不采集任何关于 UIWebView / WKWebView 的统计信息
+ (void)enableAllWebViews:(BOOL)enable;
// 设置为 YES, 将启用 HashTag
+ (void)enableHybridHashTag:(BOOL)enable;
// 是否开启了trackingWebView选项
+ (BOOL)isTrackingWebView;

// 设置是否发送元素的展现次数（浏览量、曝光量）
+ (void)setImp:(BOOL)imp;

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

// 设置数据后台服务器地址
+ (void)setHybridJSSDKUrlPrefix:(NSString*)urlPrefix;

// 设置zone信息
+ (void)setZone:(NSString *)zone;


+ (NSString *)getDeviceId;
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
 设置页面级变量

 @param variable ：页面级变量, 变量不能为nil
 @param viewController ： 目标 UIViewController
 */
+ (void)setPageVariable:(NSDictionary<NSString *, NSObject *> *)variable
       toViewController:(UIViewController *)viewController;
/**
 页面级变量
 
 @param key : 变量名称, 变量名称不能为nil或者""
 @param stringValue : 字符串变量, 变量不能为nil或者""
 @param viewController : 目标 UIViewController
 */
+ (void)setPageVariableWithKey:(NSString *)key
                andStringValue:(NSString *)stringValue
              toViewController:(UIViewController *)viewController;
/**
 页面级变量
 
 @param key : 变量名称, 变量名称不能为nil或者""
 @param numberValue : 数值类型变量, 变量不能为nil
 @param viewController : 目标 UIViewController
 */
+ (void)setPageVariableWithKey:(NSString *)key
                andNumberValue:(NSNumber *)numberValue
              toViewController:(UIViewController *)viewController;
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

// 以下方法将来有可能会废弃, 请谨慎使用
+ (void)setAppVariable:(NSDictionary<NSString *, NSObject *> *)variable;
+ (void)setAppVariableWithKey:(NSString *)key andStringValue:(NSString *)stringValue;
+ (void)setAppVariableWithKey:(NSString *)key andNumberValue:(NSNumber *)numberValue;

// 以下 plugin method 请客户不要调用
+ (void)trackPageWithPageName:(NSString *)pageName pageTime:(NSString *)pageTime; /* plugin method */
+ (void)setPageVariable:(NSDictionary<NSString *, NSObject *> *)variable
                 toPage:(NSString *)page; /* plugin method */
+ (void)setPluginUserId:(NSString *)userId; /* plugin method */
+ (void)clearPluginUserId; /* plugin method */
@end


// 该属性setter方法均使用 objc_setAssociatedObject实现
// 如果是自定义的View建议优先使用重写getter方法来实现 以提高性能
@interface UIView(GrowingAttributes)

// 手动标识该view不要追踪
@property (nonatomic, assign) BOOL      growingAttributesDonotTrack;

// 手动标识该view不要追踪它的值，默认是NO，特别的UITextView，UITextField，UISearchBar默认是YES
@property (nonatomic, assign) BOOL      growingAttributesDonotTrackValue;

// 手动标识该view的取值  比如banner广告条的id 可以放在banner按钮的任意view上
@property (nonatomic, copy)   NSString* growingAttributesValue;

// 手动标识SDCycleScrollView组件的bannerIds  如若使用,请在创建SDCycleScrollView实例对象后,立即赋值;(如果不进行手动设置,SDK默认会采集banner的imageName或者imageURL)
@property (nonatomic, strong) NSArray<NSString *>
                                      * growingSDCycleBannerIds;

// 手动标识该view的附加属性  该值可被子节点继承
@property (nonatomic, copy)   NSString* growingAttributesInfo;

// 手动标识该view的tag
// 这个tag必须是全局唯一的，在代码结构改变时也请保持不变
// 这个tag最好是常量，不要包含流水id、SKU-id、商品名称等易变的信息
// 请不要轻易设置这个属性，除非该view在view-tree里的位置不稳定，或者该view在软件的不同版本的view-tree里的位置不一致
@property (nonatomic, copy)   NSString* growingAttributesUniqueTag;

@end

// 该属性setter方法均使用 objc_setAssociatedObject实现
// 如果是自定义的UIViewController不要使用重写getter方法来实现,因为SDK在set方法内部有逻辑处理
@interface UIViewController(GrowingAttributes)

// 手动标识该vc的附加属性  该值可被子节点继承
@property (nonatomic, copy)   NSString* growingAttributesInfo;

// 手动标识该页面的标题，必须在该UIViewController显示之前设置
@property (nonatomic, copy)   NSString* growingAttributesPageName;

@end



