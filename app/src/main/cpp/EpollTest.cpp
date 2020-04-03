//
// Created by yan xianwei on 2020/4/3.
//


#include <jni.h>
#include <sys/wait.h>
#include <android/log.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <android/log.h>

#include <iostream>
#include <cstdint>
#include <memory.h>
#include <cstring>
#include <cerrno>
#include <sys/eventfd.h>
#include <sys/epoll.h>

#ifndef NATIVEEPOLL_EPOLLTEST_H
#define NATIVEEPOLL_EPOLLTEST_H

#endif //NATIVEEPOLL_EPOLLTEST_H


#define TAG        "epollTest"
#define LOGI(...)    __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...)    __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGW(...)    __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...)    __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

int efd;

int epoll_fd;
extern "C"
JNIEXPORT void JNICALL
Java_com_yan_nativeepoll_EpollTest_init(JNIEnv *env, jobject thiz) {
    // 进程/线程间通信的文件(非真实存在)
    efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

    // epoll实例
    epoll_fd = epoll_create(1);
    struct epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    //epoll的触发模式分为两种：水平触发和边缘触发。水平触发的意思是只要被监听的fd中存在数据，epoll_wait就一直返回，
    // 只有将fd中的数据读出，才会再次阻塞，等待下次写入再唤醒。边缘触发是只有向fd中写入数据，才会触发epoll_wait返回，
    // epoll_wait返回后，不必读出fd中的数据，因为不管fd中是否有数据，epoll_wait都会再次等待有人写入。
    // 边缘 EPOLLIN | EPOLLET
    event.events = EPOLLIN;
    event.data.fd = efd;
    // 通过epoll 管理event EPOLL_CTL_ADD(添加) EPOLL_CTL_DEL(删除)
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, efd, &event);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_yan_nativeepoll_EpollTest_await(JNIEnv *env, jobject thiz, jlong timeout) {
    struct epoll_event events[1];
    // timeout 超时时间，到了会唤醒
    int event_count = epoll_wait(epoll_fd, events, 1, timeout);
    if (events[0].data.fd == efd) {
        if (events[0].events == EPOLLIN) {
            uint64_t val;
            read(efd, &val, sizeof(uint64_t));
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yan_nativeepoll_EpollTest_wakeUp(JNIEnv *env, jobject thiz) {
    uint64_t val = 1;
    write(efd, &val, sizeof(val));
}

