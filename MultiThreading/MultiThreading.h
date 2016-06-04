#ifndef GRAPHIC_LIBRARY_MULTITHREADIMG_H
#define GRAPHIC_LIBRARY_MULTITHREADIMG_H

#include <queue>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>
#include <future>

namespace MultiThreading {

    template <typename T>
    class SyncQueue {
    public:
        class TakingElementFromEmptyQueueException;
        SyncQueue();
        ~SyncQueue();
        void push(T const &);
        T front();
        void pop();
        bool tryPop(int milliseconds, T &result);
        std::size_t size();
        bool empty();

    private:
        std::atomic<T*> mFront;
        std::queue<T> mQueue;
        std::mutex mMutex;
        std::condition_variable mAbleToPop;
    };

    template <typename T>
    class SyncQueue<T>::TakingElementFromEmptyQueueException : std::exception {
    public:
        virtual const char* what() const throw();
    };


    class ThreadPool {
    public:
        ThreadPool(int threads = -1);

        void addTask(std::function<void()> const &task);

        ~ThreadPool();

    private:
        const int TIME_OUT = 100;

        void doWork();

        std::vector<std::thread> mThreads;
        SyncQueue<std::function<void()>> mQueue;
        std::atomic<bool> mFinished;
    };
}


#include "SyncQueue.cpp"

#endif // GRAPHIC_LIBRARY_MULTITHREADIMG_H
