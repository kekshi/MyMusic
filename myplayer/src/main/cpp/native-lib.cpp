#include <jni.h>
#include <string>

extern "C"  //用 C 语言编译
JNIEXPORT jstring  //导入 jstring 方法
JNICALL  //JNI 调用。  方法命名规则为：Java + 包名 + 类名 + 方法名，"."换为下划线
Java_com_test_myplayer_Demo_stringFromJNI(
        JNIEnv *env, // env JNI层的变量，可以用来操作线程
        jobject /* this */) {
    std::string hello = "welcome to ffmpeg world";
    return env->NewStringUTF(hello.c_str());
}
