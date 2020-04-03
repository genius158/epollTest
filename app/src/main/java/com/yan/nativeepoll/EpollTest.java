package com.yan.nativeepoll;

/**
 * @author Bevan (Contact me: https://github.com/genius158)
 * @since 2020/4/3
 */
public class EpollTest {
    static {
        System.loadLibrary("epoll_test");

    }
    public static native void init();
    public static native void await(long timeMillis);
    public static native void wakeUp();
}
