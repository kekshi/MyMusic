//
// Created by admin on 2019/1/4.
//

#ifndef MYMUSIC_WLAUDIO_H
#define MYMUSIC_WLAUDIO_H

extern "C" {
#include <libavcodec/avcodec.h>
};


class WlAudio {
public:
    //流的索引
    int streamIndex = -1;
    AVCodecParameters *codecpar = NULL;
    AVCodecContext *avCodecContext = NULL;
public:
    WlAudio();

    ~WlAudio();
};


#endif //MYMUSIC_WLAUDIO_H
