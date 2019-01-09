//
// Created by admin on 2019/1/4.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    queue = new WlQueue(playstatus);
}

WlAudio::~WlAudio() {

}
