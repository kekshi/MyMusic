//
// Created by admin on 2019/1/4.
//

#include "WlFFmpeg.h"

WlFFmpeg::WlFFmpeg(WlPlaystatus *playstatus, WlCallJava *callJava, const char *url) {

    this->callJava = callJava;
    this->url = url;
    this->playstatus = playstatus;

}

//解码回调，调用 decodeFFmpegThread() 方法实现循环解析。
void *decodeFFmpeg(void *data) {
    WlFFmpeg *fFmpeg = (WlFFmpeg *) (data);
    fFmpeg->decodeFFmpegThread();
    pthread_exit(&fFmpeg->decodeThread);
}

//创建解析线程来解析数据，传入回调方法 decodeFFmpeg
void WlFFmpeg::parpared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

//实际解码操作的方法
void WlFFmpeg::decodeFFmpegThread() {
    //注册解码器并初始化网络
    av_register_all();
    avformat_network_init();
    //打开文件或网络流
    formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, url, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGE("can not open url :%s", url);
        }
        return;
    }
    //获取流信息
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not find streams from url :%s", url);
        }
        return;
    }

    for (int i = 0; i < formatContext->nb_streams; i++) {
        //获取音频流
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (audio == NULL) {
                //传入音频流中的采样率
                audio = new WlAudio(playstatus, formatContext->streams[i]->codecpar->sample_rate, callJava);
                audio->streamIndex = i;

                audio->codecpar = formatContext->streams[i]->codecpar;
                audio->duration = formatContext->duration / AV_TIME_BASE;
                audio->time_base = formatContext->streams[i]->time_base;
            }
        }
    }
    //通过 ID 获取解码器
    AVCodec *avCodec = avcodec_find_decoder(audio->codecpar->codec_id);
    if (!avCodec) {
        if (LOG_DEBUG) {
            LOGE("can not find decoder")
        }
        return;
    }
    //利用解码器创建解码器上下文
    audio->avCodecContext = avcodec_alloc_context3(avCodec);
    if (!audio->avCodecContext) {
        if (LOG_DEBUG) {
            LOGE("can not alloc new decoder Context")
        }
        return;
    }
    if (avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not fill decoder Context")
        }
        return;
    };
    //打开解码器
    if (avcodec_open2(audio->avCodecContext, avCodec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("can not open audio streams")
        }
        return;
    }
    callJava->onCallParpared(CHILD_THREAD);
}

//开始解码
void WlFFmpeg::start() {
    if (audio == NULL) {
        if (LOG_DEBUG) {
            LOGE("audio is null")
        }
        return;
    }

    //在解码之前是阻塞的，因此不影响性能
    audio->play();

    // int count = 0;
    //暂时写个死循环
    while (playstatus != NULL && !playstatus->exit) {
        //读取音频帧
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(formatContext, avPacket) == 0) {
            if (avPacket->stream_index == audio->streamIndex) {
                //   count++;
                //   if (LOG_DEBUG) {
                //      LOGE("解码第 %d 帧", count);
                //  }
                //释放资源
//                av_packet_free(&avPacket);
//                av_free(avPacket);
//                avPacket = NULL;
                //不直接释放资源，加入队列
                audio->queue->putAVPacket(avPacket);
            } else {
                //释放资源
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            //释放资源
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            while (playstatus != NULL && !playstatus->exit) {
                if (audio->queue->getQueueSize() > 0) {
                    continue;
                } else {
                    playstatus->exit = true;
                    break;
                }
            }
        }
    }

    //模拟出队，仅用来测试
//    while (audio->queue->getQueueSize() > 0) {
//        AVPacket *avPacket = av_packet_alloc();
//        audio->queue->getAVPacket(avPacket);
//        av_packet_free(&avPacket);
//        av_free(avPacket);
//        avPacket = NULL;
//    }
    if (LOG_DEBUG) {
        LOGD("解码完成");
    }
}

void WlFFmpeg::pause() {
    if (audio != NULL) {
        audio->pause();
    }
}

void WlFFmpeg::resume() {
    if (audio != NULL) {
        audio->resume();
    }
}
