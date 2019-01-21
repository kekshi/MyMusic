//
// Created by admin on 2019/1/4.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus, int sample_rate) {
    this->playstatus = playstatus;
    queue = new WlQueue(playstatus);
    buffer = (uint8_t *) (av_malloc(sample_rate * 2 * 2));
    this->sample_rate = sample_rate;
}

WlAudio::~WlAudio() {

}

void *decodePlay(void *data) {
    WlAudio *wlAudio = (WlAudio *) (data);
    wlAudio->initOpenSLES();
    pthread_exit(&wlAudio->thread_play);
}

void WlAudio::play() {

    pthread_create(&thread_play, NULL, decodePlay, this);
}

//FILE *outFile = fopen("/mnt/shared/Other/music.pcm", "w");

/**
 * 重采样的方法
 * */
int WlAudio::resampleAudio() {

    //如果没退出
    while (playstatus != NULL && !playstatus->exit) {
        //avPacket 分配内存空间
        avPacket = av_packet_alloc();
        if (queue->getAVPacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        //将 avPacket 放入解码器解码
        ret = avcodec_send_packet(avCodecContext, avPacket);
        if (ret != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        //avFrame 分配内存空间
        avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);
        //0成功 进行重采样
        if (ret == 0) {

            if (avFrame->channels > 0 && avFrame->channel_layout == 0) {
                //根据声道数返回声道布局
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);
            } else if (avFrame->channels == 0 && avFrame->channel_layout > 0) {
                //根据声道布局返回声道数
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }

            SwrContext *swrContext = NULL;
            swrContext = swr_alloc_set_opts(
                    NULL,
                    AV_CH_LAYOUT_STEREO,
                    AV_SAMPLE_FMT_S16,
                    avFrame->sample_rate,
                    avFrame->channel_layout,
                    (AVSampleFormat) (avFrame->format),
                    avFrame->sample_rate,
                    NULL, NULL);
            if (!swrContext || swr_init(swrContext) < 0) {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;
                if (swrContext != NULL) {
                    swr_free(&swrContext);
                    swrContext = NULL;
                }
                continue;
            }
            int nb = swr_convert(
                    swrContext,
                    &buffer,
                    avFrame->nb_samples,
                    (const uint8_t **) (avFrame->data),
                    avFrame->nb_samples);
            //重新获取立体声声道数
            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            data_size = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

//            fwrite(buffer, 1, data_size, outFile);

            if (LOG_DEBUG) {
                LOGE("数据大小是 %d", data_size);
            }

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            swr_free(&swrContext);
            swrContext = NULL;
            //获取数据成功之后跳出循环
            break;
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }
    }

    return data_size;
}


//循环获取 PCM 数据，播放完后退出
void pcmBufferCallback(SLAndroidSimpleBufferQueueItf bf, void *context) {
    WlAudio *wlAudio = (WlAudio *) (context);
    if (wlAudio != NULL) {
        int buffSize = wlAudio->resampleAudio();
        if (buffSize > 0) {
            //入队
            (*wlAudio->pcmBufferQueue)->Enqueue(wlAudio->pcmBufferQueue, (char *) wlAudio->buffer, buffSize);
        }
    }
}

void WlAudio::initOpenSLES() {
    SLresult result;
    // 第一步：创建引擎
    result = slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    //第二步：实现（Realize）engineObject 接口对象实例化
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    //第三部：通过 engineObject 的 GetInterface 方法得到 engineItf 引擎对象
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineItf);

    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    //创建混音器（参数依次为：引擎对象，SLObjectItf，变音 ID 效果的个数，SLInterfaceID，属性）步骤同上，配置参照源码
    result = (*engineItf)->CreateOutputMix(engineItf, &outputMixObject, 1, mids, mreq);
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputEnvironmentalReverbItf);
    if (SL_RESULT_SUCCESS == result) {
        //设置混音器环境和属性
        result = (*outputEnvironmentalReverbItf)->SetEnvironmentalReverbProperties(outputEnvironmentalReverbItf,
                                                                                   &reverbSettings);
    }


    SLDataLocator_AndroidBufferQueue androidBufferQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,//固定的
                                                           2};//缓冲队列里的缓冲数目
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM, //规定类型是 PCM
                            2, //设置声道为2（立体声）
                            getCurrentSampleReteForOpensles(sample_rate), //设置采样率为44100hz的频率（根据音频文件采样率来设置）
                            SL_PCMSAMPLEFORMAT_FIXED_16, //采样位数为16位
                            SL_PCMSAMPLEFORMAT_FIXED_16, //容器大小，和采样大小保持一致就不会失真
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, //设置声道，2声道为 前左和前右
                            SL_BYTEORDER_LITTLEENDIAN};//播放结束的标识

    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSource slDataSource = {&androidBufferQueue, &pcm};
    SLDataSink audioSink = {&outputMix, NULL};

    //功能清单，我们要让播放器支持的功能
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    //通过引擎接口对象创建播放器
    (*engineItf)->CreateAudioPlayer(engineItf, &pcmPlayerObject, &slDataSource, &audioSink, 1, ids, req);
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    //得到播放器 player 接口实例
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pclPlay);
    //得到缓存列队 BufferQueue 实例
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    //通过 pcmBufferQueue 注册回调
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallback, this);

    //设置播放状态
    (*pclPlay)->SetPlayState(pclPlay, SL_PLAYSTATE_PLAYING);
    //执行一次回调
    pcmBufferCallback(pcmBufferQueue, this);
}

//根据采样率返回 OpenSLES 中的采样率
int WlAudio::getCurrentSampleReteForOpensles(int sample_rate) {
    int rate = 0;
    switch (sample_rate) {
        case 8000:
            rate = SL_SAMPLINGRATE_8;
            break;
        case 11025:
            rate = SL_SAMPLINGRATE_11_025;
            break;
        case 12000:
            rate = SL_SAMPLINGRATE_12;
            break;
        case 16000:
            rate = SL_SAMPLINGRATE_16;
            break;
        case 22050:
            rate = SL_SAMPLINGRATE_22_05;
            break;
        case 24000:
            rate = SL_SAMPLINGRATE_24;
            break;
        case 32000:
            rate = SL_SAMPLINGRATE_32;
            break;
        case 44100:
            rate = SL_SAMPLINGRATE_44_1;
            break;
        case 48000:
            rate = SL_SAMPLINGRATE_48;
            break;
        case 64000:
            rate = SL_SAMPLINGRATE_64;
            break;
        case 88200:
            rate = SL_SAMPLINGRATE_88_2;
            break;
        case 96000:
            rate = SL_SAMPLINGRATE_96;
            break;
        case 192000:
            rate = SL_SAMPLINGRATE_192;
            break;
        default:
            rate = SL_SAMPLINGRATE_44_1;
    }
    return rate;
}
