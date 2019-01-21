//
// Created by admin on 2019/1/4.
//

#ifndef MYMUSIC_WLAUDIO_H
#define MYMUSIC_WLAUDIO_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
};

#include "WlQueue.h"
#include "WlPlaystatus.h"

class WlAudio {
public:
    //流的索引
    int streamIndex = -1;
    AVCodecParameters *codecpar = NULL;
    AVCodecContext *avCodecContext = NULL;
    WlQueue *queue = NULL;
    WlPlaystatus *playstatus = NULL;

    //用来重采样的线程
    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    //解码出的音频帧对象
    AVFrame *avFrame = NULL;
    //返回值
    int ret = -1;
    uint8_t *buffer = NULL;
    int data_size=0;

public:
    WlAudio(WlPlaystatus *playstatus);

    ~WlAudio();

    void play();

    int resampleAudio();
};


#endif //MYMUSIC_WLAUDIO_H
