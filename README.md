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
| track|<nobr>(final String eventName, @Nullable final Double number, @Nullable final String strEventJson, final Callback callback)</nobr>|<nobr> 打点函数</nobr>|
| trackPage | (final String pageName, final String lastPage, final long ptm, Callback callback)| 页面打点 |
| trackPage | (final String pageName, final long ptm, Callback callback)| 页面打点 |
| setUserId | (final String newUserId, final Callback callback)| 设置用户id |
| cleanUserId | (Callback callback)| 清除id |
| setAppVariable |(final String strVariableJson, Callback callback) | <nobr>第一个参数为 json String </nobr> |
| setEvarVariable | (final String strVariableJson, Callback callback) |  参数1为json String   |
| setPeopleVariable | (final String strVariableJson, Callback callback)| 参数1为json String |
            
### 五、JS中调用方式：
* 在定义Component之前引入  

		import {
		  NativeModules
		} from 'react-native';
		
* 之后就可以使用GrowingIO的方法,所有方法的最后一个参数为一个fucntion,funtion为单参数切类型为String的回掉函数。当参数为 GrowingIO.CALLBACK_SUCCESS 时表示调用成功，否则为错误信息。

			NativeModules.GrowingIO.track 
			....
	

* 例如在js中调用打点函数：

	    NativeModules.GrowingIO.track("ggl", 
	      100.00,
	      "{\"ke1\": \"heel\",\"ke2\": 100,\"struckt\": {\"kk1\": \"heal\",\"kk2\": \"heal\"}}",
	      (msg) => {
	          if(msg == NativeModules.GrowingIO.CALLBACK_SUCCESS)
	          {
	            Alert.alert('SUCCESS');
	          }else{
	            Alert.alert(msg);
	          }
	        }
	      );
	      
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
        

	      
