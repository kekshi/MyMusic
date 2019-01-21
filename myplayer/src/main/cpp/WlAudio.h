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
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

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


    //OpenSL ES 相关变量
    //引擎接口对象实例
    SLObjectItf engineObject = NULL;
    SLEngineItf engineItf = NULL;

    //混音器相关对象
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputEnvironmentalReverbItf = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //播放器相关对象
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pclPlay = NULL;

    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;
public:
    WlAudio(WlPlaystatus *playstatus);

    ~WlAudio();

    void play();

    int resampleAudio();

    //初始化 OpenSL ES
    void initOpenSLES();
};


#endif //MYMUSIC_WLAUDIO_H
