package com.growingio.android.rn;

import android.util.Log;

import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.growingio.android.sdk.collection.GrowingIO;

import org.json.JSONException;

/**
 * author CliffLeopard
 * time   2017/8/31:上午10:38
 * email  gaoguanling@growingio.com
 */

public class GrowingIOModule extends ReactContextBaseJavaModule implements LifecycleEventListener, GioRnApi {

    private final String ModuleName = "GrowingIO";
    private final String TAG = "GIO.GrowingIOModule";

    public GrowingIOModule(ReactApplicationContext reactContext) {
        super(reactContext);
        reactContext.addLifecycleEventListener(this);
    }

    @Override
    public String getName() {
        return ModuleName;
    }

    @ReactMethod
    @Override
    public void track(String eventId, ReadableMap eventLevelVariable) {
        try {
            GrowingIO.getInstance().track(eventId, RnUtils.convertMapToJson(eventLevelVariable));
        } catch (JSONException exp) {
            Log.e(TAG, "trackWithNumber:json格式解析错误");
        }
    }

    @ReactMethod
    @Override
    public void trackWithNumber(String eventId, Double number, ReadableMap eventLevelVariable) {
        try {
            GrowingIO.getInstance().track(eventId, number, RnUtils.convertMapToJson(eventLevelVariable));
        } catch (JSONException exp) {
            Log.e(TAG, "trackWithNumber:json格式解析错误");
        }
    }

    @ReactMethod
    @Override
    public void page(String page) {
        GrowingIO.getInstance().trackPage(page);
    }

    @ReactMethod
    @Override
    public void setPageVariable(String page, ReadableMap pageLevelVariables) {
        try {
            GrowingIO.getInstance().setPageVariable(page, RnUtils.convertMapToJson(pageLevelVariables));
        } catch (JSONException exp) {
            Log.e(TAG, "setPageVariable:json格式解析错误");
        }
    }

    @ReactMethod
    @Override
    public void setEvar(ReadableMap conversionVariables) {
        try {
            GrowingIO.getInstance().setEvar(RnUtils.convertMapToJson(conversionVariables));
        } catch (JSONException exp) {
            Log.e(TAG, "setEvar:json格式解析错误");
        }
    }

    @ReactMethod
    @Override
    public void setPeopleVariable(ReadableMap peopleVariables) {
        try {
            GrowingIO.getInstance().setPeopleVariable(RnUtils.convertMapToJson(peopleVariables));
        } catch (JSONException exp) {
            Log.e(TAG, "setPeopleVariable:json格式解析错误");
        }
    }

    @ReactMethod
    @Override
    public void setUserId(String userId) {
        GrowingIO.getInstance().setUserId(userId+System.currentTimeMillis());
    }

    @ReactMethod
    @Override
    public void clearUserId() {
        GrowingIO.getInstance().clearUserId();
    }


    @Override
    public void onHostResume() {

    }

    @Override
    public void onHostPause() {

    }

    @Override
    public void onHostDestroy() {

    }
}