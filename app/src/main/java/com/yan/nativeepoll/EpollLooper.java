package com.yan.nativeepoll;

/**
 * @author Bevan (Contact me: https://github.com/genius158)
 * @since 2020/4/3
 */
public class EpollLooper {
    private static EpollHandle handler = new EpollHandle();

    public static void prepare() {
        EpollTest.init();
        new Thread(() -> {
            while (true) {
                EpollTest.await(-1);
                handler.dispatch();
            }
        }).start();
    }

    public static void post(CallBack callBack) {
        handler.post(callBack);
    }

    static class EpollHandle {
        private CallBack onCall;

        void post(CallBack onCall) {
            this.onCall = onCall;
            EpollTest.wakeUp();
        }

        private void dispatch() {
            if (onCall != null) {
                onCall.onCall();
            }
        }
    }

    interface CallBack {
        void onCall();
    }

}
