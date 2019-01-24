//
// Created by admin on 2019/1/4.
//

#include "WlCallJava.h"

WlCallJava::WlCallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj) {

    this->jvm = javaVM;
    this->jniEnv = env;
    this->jobj = env->NewGlobalRef(*obj); //将 jobj 设置为全局的

    jclass clz = env->GetObjectClass(jobj);

    if (!clz) {
        if (LOG_DEBUG) {
            LOGE("get jclass wrong");
            return;
        }
    }

    //(ILjava/lang/String;)V  整型、字符串类型，没有返回值
    //获取 onError 的方法ID
    jmid_parpared = env->GetMethodID(clz, "onCallParpared", "()V");

    jmid_load = env->GetMethodID(clz, "onCallLoad", "(Z)V");
}

WlCallJava::~WlCallJava() {

}

void WlCallJava::onCallParpared(int type) {
    //主线程执行 或子线程执行
    if (type == MAIN_THREAD) {
        //执行 onCallParpared 方法调用
        jniEnv->CallVoidMethod(jobj, jmid_parpared);
    } else if (type == CHILD_THREAD) {
        JNIEnv *env;
        if (jvm->AttachCurrentThread(&env, 0) != JNI_OK) {
            if (LOG_DEBUG) {
                LOGE("get child thread jnienv wrong");
                return;
            }
        };

        env->CallVoidMethod(jobj, jmid_parpared);

        jvm->DetachCurrentThread();//取消关联当前线程
    }
}

void WlCallJava::onCallLoad(int type, bool load) {
    //主线程执行 或子线程执行
    if (type == MAIN_THREAD) {
        //执行 onCallLoad 方法调用 , 第三个值是参数
        jniEnv->CallVoidMethod(jobj, jmid_load, load);
    } else if (type == CHILD_THREAD) {
        JNIEnv *env;
        if (jvm->AttachCurrentThread(&env, 0) != JNI_OK) {
            if (LOG_DEBUG) {
                LOGE("get child thread jnienv wrong");
                return;
            }
        };

        env->CallVoidMethod(jobj, jmid_load, load);

        jvm->DetachCurrentThread();//取消关联当前线程
    }
}
