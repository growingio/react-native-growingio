# GrowingIO React Native SDK Reference

[![npm version](https://img.shields.io/npm/v/react-native-growingio.svg?style=flat)](https://www.npmjs.com/package/react-native-growingio)
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/growingio/react-native-growingio/blob/master/LICENSE)

___

### 一、说明
react-native-growingio 用于RN开发者使用打点的方式采集数据。

### 二、引入

* `npm install --save https://github.com/growingio/react-native-growingio.git`
* `npm install`
* `react-native link react-native-growingio`

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
            [Growing startWithAccountId:@"您的项目ID"];

            // 其他配置
            // 开启Growing调试日志 可以开启日志
            // [Growing setEnableLog:YES];
        }


* [添加官网配置](https://docs.growingio.com/SDK/iOS.html), 从步骤三开始

### android
* 在android project 级别的build.gradle下添加如下依赖: `classpath 'com.growingio.android:vds-gradle-plugin:OP-2.0.0'`

* 在android app module 级别的build.gradle下添加如下代码: `apply plugin: 'com.growingio.android'`
			    
* 在Application中的onCreate方法中初始化：

        GrowingIO.startWithConfiguration(this, new Configuration()
                .useID()
                .trackAllFragments()
                .setChannel("**应用商店"));
* [添加官网配置](https://www.growingio.com)
    
### 四、方法说明


| 函数名 | 参数 | 说明|
|:-----:|-----|:-------:|
| track|<nobr>(String eventId, Object eventLevelVariable(optional))</nobr>|<nobr> 打点函数</nobr>|
| trackWithNumber|<nobr>(String eventId, Number number, Object eventLevelVariable(optional))</nobr>|<nobr> 打点函数</nobr>|
| page | (String page)| 页面打点 |
| serUserId | (String userId)| 设置用户id |
| cleanUserId | | 清除id |
| setPageVariable |(String page, Object pageLevelVariables) | <nobr>页面变量 </nobr> |
| setEvar | (Object conversionVariables) |   转化变量  |
| setPeopleVariable | (Object peopleVariables)| 用户变量 |
            
### 五、JS中调用方式：
* 在定义Component之前引入  

		import {
		  NativeModules
		} from 'react-native';
		
* 之后就可以使用GrowingIO的方法


* 例如在js中调用打点函数：

          NativeModules.GrowingIO.track("ggl", {"key1":"100"});
	      
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
 		
 		
4. demo 可见 examples/HelloDemo

	      
