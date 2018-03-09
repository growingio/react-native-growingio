package com.growingio.android.rn;

import com.facebook.react.bridge.ReadableMap;

/**
 * author CliffLeopard
 * time   2018/2/7:下午1:29
 * email  gaoguanling@growingio.com
 */

public interface GioRnApi {
    void track(String eventId, ReadableMap eventLevelVariable);

    void trackWithNumber(String eventId, Double number, ReadableMap eventLevelVariable);

    void page(String page);

    void setPageVariable(String page, ReadableMap pageLevelVariables);

    void setEvar(ReadableMap conversionVariables);

    void setPeopleVariable(ReadableMap peopleVariables);

    void setUserId(String userId);

    void clearUserId();
}
