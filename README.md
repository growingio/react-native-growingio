# GrowingIO React Native SDK Reference

[![npm version](https://img.shields.io/npm/v/react-native-growingio.svg?style=flat)](https://www.npmjs.com/package/react-native-growingio)
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/growingio/react-native-growingio/blob/master/LICENSE)

___

### 一、说明
react-native-growingio 用于RN开发者手动发送数据。

### 二、引入

* `npm install --save https://github.com/growingio/react-native-growingio.git`
* `npm install`
* `react-native link react-native-growingio`
#### iOS
* 使用CocoaPods安装GrowingCoreKit ( pod 'GrowingCoreKit', '~> 2.4.0' in your Podfile and run pod install)
* **(optional)** GrowingIO推荐您添加**AdSupport.framework**依赖库,用于来源管理激活匹配,有利于您更好的分析的数据,  
添加项目依赖库的位置在项目设置target -> 选项卡General -> Linked Frameworks and Libraries

### 三、配置
### iOS
* 如果`react-native link react-native-growingio`失败(成功则忽略此步骤),即发现Libraries中没有GrowingIORNPlugin.xcodeproj,则可手动配置;
a.打开XCode's工程中, 右键点击Libraries文件夹 ➜ Add Files to <...>
b.去node_modules ➜ react-native-growingio ➜ ios ➜ 选择 GrowingIORNPlugin.xcodeproj
c.在工程Build Phases ➜ Link Binary With Libraries中添加libGrowingIORNPlugin.a

* 添加初始化函数:
在 AppDelegate 中引入#import "Growing.h"并添加启动方法

        - (BOOL)application:(UIApplication *)application
        didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
            ...
            // 启动GrowingIO
            [Growing startWithAccountId:@"项目ID"];

            // 其他配置
            // 开启Growing调试日志 可以开启日志
            // [Growing setEnableLog:YES];
        }


* [添加官网配置](https://docs.growingio.com/sdk-20/sdk-20-api-wen-dang/ios-sdk-21-an-zhuang.html), 需要重构后的文档

### android

* 在Application中的onCreate方法中初始化：

        GrowingIO.startWithConfiguration(this, new Configuration()
                .useID()
                .trackAllFragments()
                .setChannel("**应用商店"));

*  AndroidManifest.xml以及module级别build.gradle中android defaultConfig 中添加的属性，请见官网配置。 [添加官网配置](https://docs.growingio.com/sdk-20/sdk-20-api-wen-dang/android-sdk-21-an-zhuang.html)

### 四、方法说明


| 方法名 | 参数 | 说明|
|:-----:|-----|:-------:|
| track|<nobr>(String eventId, Object eventLevelVariable(optional))</nobr>|<nobr> 自定义事件（计数器类型）</nobr>|
| trackWithNumber|<nobr>(String eventId, Number number, Object eventLevelVariable(optional))</nobr>|<nobr> 自定义事件（数值类型）</nobr>|
| setUserId | (String userId)| 设置登录用户ID |
| clearUserId | | 清除登录用户ID |
| setEvar | (Object conversionVariables) |   设置转化变量  |
| setPeopleVariable | (Object peopleVariables)| 设置用户变量 |
| setVisitor | (Object visitor)| 设置访问用户变量 |

### 五、JS中调用方式：

* 在定义Component之前引入

        import {
          NativeModules
        } from 'react-native';

* 之后就可以使用GrowingIO的方法,例如在js中调用自定义事件方法：

        NativeModules.GrowingIO.track("registerSuccess", {"gender":"male"});

### Tips

* 由于最新的ReactNative 打包gradlew存在bug，所以android在打debug包和releae包时要进行如下操作：


1. 在工程目录下

        mkdir Android/app/assets

2. 在app build.gradle  android中添加：


        sourceSets {
            main {
                assets.srcDirs = ['assets']
            }
        }

3. 在工程目录下：

        react-native bundle --platform android --dev false --entry-file App.js --bundle-output android/app/assets/index.android.bundle  --assets-dest android/app/src/main/res/


4. demo 可见 examples/App.js



