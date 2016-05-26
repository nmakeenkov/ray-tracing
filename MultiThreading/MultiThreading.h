#ifndef GRAPHIC_LIBRARY_MULTITHREADIMG_H
#define GRAPHIC_LIBRARY_MULTITHREADIMG_H

#include <queue>
#include <condition_variable>
#include <atomic>
#include <thread>

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


    template <typename T>
    class ThreadPool {
    public:
        ThreadPool(int threads) {
            //
        }
        void addTask() {
            //
        }
    private:
        void doWork() {
            //
        }
        std::vector<std::thread> mThreads;
    };
}


#include "SyncQueue.cpp"

#endif // GRAPHIC_LIBRARY_MULTITHREADIMG_H
