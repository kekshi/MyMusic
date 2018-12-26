package com.test.myplayer;

public class Demo {

    static {
        // Used to load the 'native-lib' library on application startup.
        System.loadLibrary("native-lib");
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     * 此方法由 native-lib.cpp 文件实现
     */
    public native String stringFromJNI();

}
