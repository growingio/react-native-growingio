package com.growingio.android.sdk.rn;

import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.widget.Toast;

import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.growingio.android.sdk.collection.Configuration;
import com.growingio.android.sdk.collection.GrowingIO;
import com.growingio.android.sdk.collection.SessionManager;
import com.growingio.android.sdk.utils.LogUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

import static com.growingio.android.sdk.collection.SessionManager.enterNewPage;

/**
 * author CliffLeopard
 * time   2017/8/31:上午10:38
 * email  gaoguanling@growingio.com
 */

public class GrowingIOModule extends ReactContextBaseJavaModule implements LifecycleEventListener {

    private final String ModuleName = "GrowingIO";
    private final String TAG = "GIO.GrowingIOModule";
    private final String CALLBACK_SUCCESS = "200";

    public static final String PAGE_RN_INIT = "RNInitPage";
    private final String PAGE_INIT_USERID = "RNInitUserIdPage";
    private final String PAGE_CHANGE_USERID = "RNChangeUserIdPage";
    private final String PAGE_CLEAN_USERID = "RNCleanUserIdPage";
    private String currentPageName;

    public GrowingIOModule(ReactApplicationContext reactContext) {
        super(reactContext);
        reactContext.addLifecycleEventListener(this);
    }


    @Override
    public String getName() {
        return ModuleName;
    }


    @ReactMethod
    public void init(final String accountId, @Nullable final String strOptionJson, final Callback callback) {

        if (!TextUtils.isEmpty(strOptionJson)) {
            try {
                JSONObject options = new JSONObject(strOptionJson);
                if (options.has("zone")) {
                    GrowingIO.setZone(options.optString("zone", null));
                }
                GrowingIO.getInstance()
                        .startWithConfiguration(GrowingIO.getDefaultApplication(),
                                getConfigFromOptions(options)
                                        .setProjectId(accountId));
            } catch (JSONException e) {
                if (callback != null)
                    callback.invoke("the second parameter must be null ,empty String or String of  JSONObject");
                LogUtil.d(TAG, "track fail strEventJson not json");
                return;
            }
        } else {
            GrowingIO.getInstance()
                    .startWithConfiguration(GrowingIO.getDefaultApplication(),
                            new Configuration()
                                    .setProjectId(accountId)
                                    .useID()
                                    .setURLScheme("default_url_scheme"));
        }

        enterNewPage();
        saveVisit(PAGE_RN_INIT);
        trackPage(PAGE_RN_INIT, System.currentTimeMillis());

        if (callback != null)
            callback.invoke(CALLBACK_SUCCESS);
    }

    /**
     * 打点
     *
     * @param eventName
     * @param strNumber
     * @param strEventJson 不可为嵌套格式的json
     * @param callback
     */
    @ReactMethod
    public void track(final String eventName, @Nullable final String strNumber, @Nullable final String strEventJson, @Nullable final Callback callback) {
        Double number;
        try {
            if (TextUtils.isEmpty(strNumber))
                number = null;
            else
                number = Double.valueOf(strNumber);
        } catch (NumberFormatException e) {
            if (callback != null)
                callback.invoke("the second parameter must be null ,empty String or String of  Double");
            LogUtil.d(TAG, "track fail strEventJson not json");
            return;
        }
        JSONObject eventJson;
        if (TextUtils.isEmpty(eventName)) {
            if (callback != null)
                callback.invoke("the first parameter can't be null");
            LogUtil.d(TAG, "track fail");
        }
        if (strEventJson != null && !TextUtils.isEmpty(strEventJson.trim())) {
            try {
                eventJson = new JSONObject(strEventJson);
            } catch (JSONException e) {
                if (callback != null)
                    callback.invoke("the third parameter must be null ,empty String or String of  JSONObject");
                LogUtil.d(TAG, "track fail strEventJson not json");
                return;
            }
        } else {
            eventJson = null;
        }

        GrowingIO.getInstance().track(eventName, number, eventJson);
        if (callback != null)
            callback.invoke(CALLBACK_SUCCESS);
        LogUtil.d(TAG, "track success:" + eventName + "  " + number.toString() + "  " + eventJson == null ? "" : eventJson.toString() + "  ");

    }

    public void trackPage(final String pageName, final String lastPage, final long ptm, Callback callback) {
        if (trackPage(pageName, lastPage, ptm)) {
            callback.invoke("the third parameter must be null ,empty String or String of  JSONObject");
            LogUtil.d(TAG, "trackPage fail");
        } else {
            callback.invoke(CALLBACK_SUCCESS);
            LogUtil.d(TAG, "trackPage success");
        }
    }

    @ReactMethod
    public void trackPage(final String pageName, final String strPtm, Callback callback) {
        if (TextUtils.isEmpty(strPtm)) {
            if (callback != null)
                callback.invoke("the second parameter neither be empty nor null");
            LogUtil.d(TAG, "track fail strEventJson not json");
            return;
        }
        try {
            Long ptm = Long.valueOf(strPtm);
            trackPage(pageName, null, ptm.longValue(), callback);
        } catch (NumberFormatException e) {
            if (callback != null)
                callback.invoke("the second parameter must be String of long");
            LogUtil.d(TAG, "track fail strPtm not String of long");
        }
    }


    @ReactMethod
    public void saveVisit(String pageName, Callback callback) {
        if (saveVisit(pageName)) {
            callback.invoke(CALLBACK_SUCCESS);
            LogUtil.d(TAG, "saveVisit success ");
        } else {
            callback.invoke("the first parameter cant't be empty");
            LogUtil.d(TAG, "saveVisit fail");
        }
    }


    @ReactMethod
    public void setUserId(final String newUserId, final Callback callback) {
        try {
            if (TextUtils.isEmpty(GrowingIO.getInstance().getUserId())) {
                trackPage(PAGE_INIT_USERID, System.currentTimeMillis());
            }

            String oldUserId = GrowingIO.getInstance().getUserId();
            if (TextUtils.isEmpty(oldUserId) && TextUtils.isEmpty(newUserId)) {
                callback.invoke(CALLBACK_SUCCESS);
                LogUtil.d(TAG, "serUserId success");
                return;
            } else if (newUserId.equals(oldUserId)) {
                callback.invoke(CALLBACK_SUCCESS);
                LogUtil.d(TAG, "serUserId success");
                return;
            }
            GrowingIO.getInstance().setUserId(newUserId);
            /**
             * 如果userId当前为空则发送一个新的PV, pagename建议为："CordovaInitUserIdPage",ptm为当前时间
             * 如果userId已经有值则发送一个新Visit和一个新的PV，pagename建议为："CordovaChangeUserIdPage"，ptm为当前时间
             * 如果userId已经有值并且新设置的值是一个空字符串，则发送一个新的Visit和新的PV，pagename建议为："CordovaCleanUserIdPage", ptm为当前时间
             */
            if (TextUtils.isEmpty(oldUserId)) {
                trackPage(PAGE_INIT_USERID, System.currentTimeMillis());
                callback.invoke(CALLBACK_SUCCESS);
                LogUtil.d(TAG, "serUserId success");
            }
            if (TextUtils.isEmpty(newUserId)) {
                saveVisit(PAGE_CLEAN_USERID);
                trackPage(PAGE_CLEAN_USERID, System.currentTimeMillis());
                callback.invoke(CALLBACK_SUCCESS);
                LogUtil.d(TAG, "serUserId success");
            }
            saveVisit(PAGE_CHANGE_USERID);
            trackPage(PAGE_CHANGE_USERID, System.currentTimeMillis());
            callback.invoke(CALLBACK_SUCCESS);
            LogUtil.d(TAG, "serUserId success");
            return;
        } catch (Exception e) {
            callback.invoke(e.getMessage());
            LogUtil.d(TAG, e.getMessage());
        }
    }


    @ReactMethod
    public void cleanUserId(Callback callback) {
        GrowingIO.getInstance().clearUserId();
        callback.invoke(CALLBACK_SUCCESS);
        LogUtil.d(TAG, "cleanUserId success");
    }


    @ReactMethod
    public void setAppVariable(final String strVariableJson, Callback callback) {
        try {
            JSONObject jsonVariable = new JSONObject(strVariableJson);
            GrowingIO.getInstance().setAppVariable(jsonVariable);
            callback.invoke(CALLBACK_SUCCESS);
            LogUtil.d(TAG, "setAppVariable success");
        } catch (JSONException e) {
            callback.invoke("the first parameter must be String of JSONObject");
            LogUtil.d(TAG, "setAppVariable :" + e.getMessage());
        }
    }

    @ReactMethod
    public void setEvarVariable(final String strVariableJson, Callback callback) {
        try {
            JSONObject jsonVariable = new JSONObject(strVariableJson);
            GrowingIO.getInstance().setEvar(jsonVariable);
            callback.invoke(CALLBACK_SUCCESS);
            LogUtil.d(TAG, "setEvarVariable success");
        } catch (JSONException e) {
            callback.invoke("the first parameter must be String of JSONObject");
            LogUtil.d(TAG, "setEvarVariable :" + e.getMessage());
        }
    }


    @ReactMethod
    public void setPeopleVariable(final String strVariableJson, Callback callback) {
        try {
            JSONObject jsonVariable = new JSONObject(strVariableJson);
            GrowingIO.getInstance().setPeopleVariable(jsonVariable);
            callback.invoke(CALLBACK_SUCCESS);
            LogUtil.d(TAG, "setEvarVariable success");
        } catch (JSONException e) {
            callback.invoke("the first parameter must be String of JSONObject");
            LogUtil.d(TAG, "setEvarVariable :" + e.getMessage());
        }
    }


    private boolean saveVisit(String pagename) {
        if (TextUtils.isEmpty(pagename))
            return false;
        GrowingIO.getInstance().saveVisit(pagename);
        return true;
    }

    private boolean trackPage(String pageName, String lastPage, long ptm) {
        if (TextUtils.isEmpty(pageName))
            return false;

        if (TextUtils.isEmpty(lastPage))
            currentPageName = lastPage;
        GrowingIO.getInstance().trackPage(pageName, currentPageName, ptm);
        currentPageName = pageName;
        return true;
    }

    @Override
    public Map<String, Object> getConstants() {
        final Map<String, Object> constants = new HashMap<>();
        constants.put("CALLBACK_SUCCESS", CALLBACK_SUCCESS);
        return constants;
    }

    private void trackPage(String pageName, long ptm) {
        GrowingIO.getInstance().trackPage(pageName, currentPageName, ptm);
        currentPageName = pageName;
    }

    @Override
    public void onHostResume() {
        enterNewPage();
        saveVisit(PAGE_RN_INIT);
        trackPage(PAGE_RN_INIT, currentPageName, System.currentTimeMillis());
        GrowingIO.getInstance().saveVisit(PAGE_RN_INIT);
    }


    @Override
    public void onHostPause() {

    }

    @Override
    public void onHostDestroy() {

    }

    private Configuration getConfigFromOptions(JSONObject options) {
        Configuration config = new Configuration();
        if (options.has("growingio_url_scheme")) {
            config.setURLScheme(options.optString("growingio_url_scheme", "default_url_scheme"));
        } else {
            config.setURLScheme("default_url_scheme");
        }

        if (options.has("channel")) {
            config.setChannel(options.optString("channel", ""));
        }

        if (options.has("useid")) {
            if (options.optBoolean("useid", true))
                config.useID();
        }

        if (options.has("debug")) {
            config.setDebugMode(options.optBoolean("debug", false));
        }

        return config;
    }


}