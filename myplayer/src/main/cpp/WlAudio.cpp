//
// Created by admin on 2019/1/4.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    queue = new WlQueue(playstatus);
    buffer = (uint8_t *) (av_malloc(44100 * 2 * 2));
}

WlAudio::~WlAudio() {

}

void *decodePlay(void *data) {
    WlAudio *wlAudio = static_cast<WlAudio *>(data);
    wlAudio->resampleAudio();
    pthread_exit(&wlAudio->thread_play);
}

void WlAudio::play() {

    pthread_create(&thread_play, NULL, decodePlay, this);
}

FILE *outFile = fopen("/mnt/shared/Other/music.pcm", "w");

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

            fwrite(buffer, 1, data_size, outFile);

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
            //break;
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

    return 0;
}

