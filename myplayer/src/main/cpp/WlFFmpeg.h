//
// Created by admin on 2019/1/4.
//

#ifndef MYMUSIC_WLFFMPEG_H
#define MYMUSIC_WLFFMPEG_H

#include "WlCallJava.h"
#include "pthread.h"
#include "WlAudio.h"

extern "C" {
#include <libavformat/avformat.h>
};

class WlFFmpeg {

public:
    WlCallJava *callJava = NULL;
    const char *url = NULL;
    pthread_t decodeThread;
    AVFormatContext *formatContext = NULL;
    WlAudio *audio = NULL;
public:
    WlFFmpeg(WlCallJava *callJava, const char *url);

    ~WlFFmpeg();

    //准备方法
    void parpared();

    //解码线程
    void decodeFFmpegThread();

    //开始播放
    void start();
};


#endif //MYMUSIC_WLFFMPEG_H
