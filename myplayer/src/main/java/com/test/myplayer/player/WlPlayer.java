package com.test.myplayer.player;

import android.text.TextUtils;
import com.test.myplayer.listener.WlOnLoadListener;
import com.test.myplayer.listener.WlOnParparedListener;
import com.test.myplayer.listener.WlOnPauseResumeListener;
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
    private WlOnLoadListener mWlOnLoadListener;
    private WlOnPauseResumeListener mWlOnPauseResumeListener;

    public WlPlayer() {
    }

    public void setWlOnParparedListener(WlOnParparedListener mWlOnParparedListener) {
        this.mWlOnParparedListener = mWlOnParparedListener;
    }

    public void setWlOnLoadListener(WlOnLoadListener wlOnLoadListener) {
        this.mWlOnLoadListener = wlOnLoadListener;
    }

    public void setWlOnPauseResumeListener(WlOnPauseResumeListener wlOnPauseResumeListener) {
        this.mWlOnPauseResumeListener = wlOnPauseResumeListener;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public void parpared() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source cannot be empty");
            return;
        }
        onCallLoad(true);
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

    public void pause() {
        n_pause();
        if (mWlOnPauseResumeListener != null) {
            mWlOnPauseResumeListener.onPause(true);
        }
    }

    public void resume() {
        n_resume();
        if (mWlOnPauseResumeListener != null) {
            mWlOnPauseResumeListener.onPause(false);
        }
    }

    /**
     * C++ 层回调 java 层的回调方法
     */
    public void onCallParpared() {
        if (mWlOnParparedListener != null) {
            mWlOnParparedListener.onParpared();
        }
    }

    /**
     * C++ 层回调 java 层的回调方法
     */
    public void onCallLoad(boolean load) {
        if (mWlOnLoadListener != null) {
            mWlOnLoadListener.onCallLoad(load);
        }
    }

    //准备方法
    private native void n_parpared(String source);

    //启动方法
    private native void n_start();

    //暂停方法
    private native void n_pause();

    //重新播放方法
    private native void n_resume();
}
