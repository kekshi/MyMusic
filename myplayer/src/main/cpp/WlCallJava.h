//
// Created by admin on 2019/1/4.
//

#ifndef MYMUSIC_WLCALLJAVA_H
#define MYMUSIC_WLCALLJAVA_H

#include <cwchar>
#include "jni.h"
#include "AndroidLog.h"

#define  MAIN_THREAD 0
#define  CHILD_THREAD 1

class WlCallJava {

public:

    JavaVM *jvm = NULL;
    jobject jobj;  //全局对象
    _JNIEnv *jniEnv;
    jmethodID jmid_parpared;
    jmethodID jmid_load;
    jmethodID jmid_timeinfo;

public:
    WlCallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj); //构造方法
    ~WlCallJava(); //析构造函数，回收对象时调用

    /**
     *  type 线程类型
     */
    void onCallParpared(int type);

    void onCallLoad(int type, bool load);

    void onCallTimeInfo(int type, int curr, int total);
};


#endif //MYMUSIC_WLCALLJAVA_H
