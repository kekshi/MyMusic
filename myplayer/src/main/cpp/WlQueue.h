//
// Created by admin on 2019/1/7.
//

#ifndef MYMUSIC_WLQUEUE_H
#define MYMUSIC_WLQUEUE_H

#include "queue"
#include "pthread.h"
#include "WlPlaystatus.h"
#include "AndroidLog.h"

extern "C" {
#include <libavcodec/avcodec.h>
};

class WlQueue {

public:
    //队列
    std::queue<AVPacket *> queuePacket;
    //条件锁
    pthread_mutex_t mutexPacket;
    //条件变量
    pthread_cond_t condPacket;

    WlPlaystatus *playstatus = NULL;

public:
    WlQueue(WlPlaystatus *playstatus);  //构造函数
    ~WlQueue();  //析构造函数，回收对象时调用

    int putAVPacket(AVPacket *packet);

    int getAVPacket(AVPacket *packet);

    int getQueueSize();
};


#endif //MYMUSIC_WLQUEUE_H
