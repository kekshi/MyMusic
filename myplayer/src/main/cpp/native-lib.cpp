#include <jni.h>
#include <string>
//导入 Android log 头文件
#include <android/log.h>

//定义打印日志(新加入)
#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"ywl5320",FORMAT,##__VA_ARGS__);
//(新加入)
extern "C" {
//说明这个是C语言，不然会报错。一定要放在代码块里
#include <libavformat/avformat.h>
}

//JNIEXPORT jstring  //导入 jstring 方法
//JNICALL  //JNI 调用。  方法命名规则为：Java + 包名 + 类名 + 方法名，"."换为下划线
//Java_com_test_myplayer_Demo_stringFromJNI(
//        JNIEnv *env, // env JNI层的变量，可以用来操作线程
//       jobject /* this */) {
//    std::string hello = "welcome to ffmpeg world";
//    return env->NewStringUTF(hello.c_str());
//}

//(新加入)
extern "C"
JNIEXPORT void JNICALL
Java_com_test_myplayer_Demo_testFfmpeg(JNIEnv *env, jobject instance) {

    av_register_all();
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                LOGI("[Video]:%s", c_temp->name);
                break;
            case AVMEDIA_TYPE_AUDIO:
                LOGI("[Audio]:%s", c_temp->name);
                break;
            default:
                LOGI("[Other]:%s", c_temp->name);
                break;
        }
        c_temp = c_temp->next;
    }
}