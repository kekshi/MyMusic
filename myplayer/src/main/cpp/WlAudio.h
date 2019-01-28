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
#include "WlCallJava.h"
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
    WlCallJava *callJava;

    //用来重采样的线程
    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    //解码出的音频帧对象
    AVFrame *avFrame = NULL;
    //返回值
    int ret = -1;
    uint8_t *buffer = NULL;
    int data_size = 0;
    int sample_rate = 0;
    //总时长
    int duration = 0;
    //基准时间
    AVRational time_base;
    //当前 AVframe 时间
    double now_time = 0;
    //当前时间
    double clock = 0;

    double last_time = 0;
    //阈值
    double threshold_time = 0.1;

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
    SLPlayItf pcmPlay = NULL;

    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;
public:
    WlAudio(WlPlaystatus *playstatus, int sample_rate, WlCallJava *callJava);

    ~WlAudio();

    void play();

    int resampleAudio();

    //初始化 OpenSL ES
    void initOpenSLES();

    int getCurrentSampleReteForOpensles(int sample_rate);

    void pause();

    void resume();
};


#endif //MYMUSIC_WLAUDIO_H
