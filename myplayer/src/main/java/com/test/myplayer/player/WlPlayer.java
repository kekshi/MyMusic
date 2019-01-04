package com.test.myplayer.player;

import android.text.TextUtils;
import com.test.myplayer.listener.WlOnParparedListener;
import com.test.myplayer.log.MyLog;

public class WlPlayer {


    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    private String source;

    private WlOnParparedListener mWlOnParparedListener;

    public WlPlayer() {
    }

    public void setmWlOnParparedListener(WlOnParparedListener mWlOnParparedListener) {
        this.mWlOnParparedListener = mWlOnParparedListener;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public void parpared() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source cannot be empty");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_parpared(source);
            }
        }).start();
    }

    public void start() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source cannot be empty");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_start();
            }
        }).start();
    }

    /**
     * C++ 层回调 java 层的回调方法
     */
    public void onCallParpared() {
        if (mWlOnParparedListener != null) {
            mWlOnParparedListener.onParpared();
        }
    }

    //准备方法
    public native void n_parpared(String source);

    //启动方法
    public native void n_start();
}
