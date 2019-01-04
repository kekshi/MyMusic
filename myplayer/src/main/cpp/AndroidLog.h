//
// Created by admin on 2018/12/28.
// 用来打印 native 层日志的头文件
//

#ifndef JNITHREADDEMO_ANDROIDLOG_H
#define JNITHREADDEMO_ANDROIDLOG_H

#endif //JNITHREADDEMO_ANDROIDLOG_H

#include <android/log.h>

//用来控制是否打印日志的开关
#define LOG_DEBUG true

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,"YW15320",FORMAT,##__VA_ARGS__);

#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"YW15320",FORMAT,##__VA_ARGS__);