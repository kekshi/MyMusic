package com.test.myplayer;

public class Demo {

    static {
        // Used to load the 'native-lib' library on application startup.
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }


    /**
     * 此方法由 native-lib.cpp 文件实现
     */
//    public native String stringFromJNI();

    public native void testFfmpeg();
}
