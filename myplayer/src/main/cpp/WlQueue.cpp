//
// Created by admin on 2019/1/7.
//

#include "WlQueue.h"
#include "AndroidLog.h"

WlQueue::WlQueue(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    //初始化
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

WlQueue::~WlQueue() {
    //释放资源
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

//生产者线程，入队操作
int WlQueue::putAVPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);

    //加入队列
    queuePacket.push(packet);

    if (LOG_DEBUG) {
        LOGD("放入一个AVPacket到队列里面，个数为：%d", queuePacket.size());
    }
    //通知消费者线程
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WlQueue::getAVPacket(AVPacket *packet) {
    //线程锁
    pthread_mutex_lock(&mutexPacket);

    // playstatus 不为空，且没退出
    while (playstatus != NULL && !playstatus->exit) {
        if (queuePacket.size() > 0) {
            AVPacket *avPacket = queuePacket.front();
            //将得到的内存引用拷贝到 packet
            if (av_packet_ref(packet, avPacket) == 0) {
                queuePacket.pop();
            }
            //拷贝后，释放 avPacket
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket == NULL;
            if (LOG_DEBUG) {
                LOGD("从列队中取出一个 avPacket ，还剩下 %d 个", queuePacket.size());
            }
            break;
        } else {
            //线程等待，此时会自动解锁
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }
    //解锁。和锁同步出现
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WlQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);
    return size;
}
