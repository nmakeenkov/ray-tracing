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


    #define Callable std::function<void()>
    class ThreadPool {
    public:
        ThreadPool(int threads = -1) : mThreads() {
            mFinished.store(false);
            if (threads < 0) {
                threads = std::thread::hardware_concurrency();
            }
            threads = std::max(1, threads);
            for (int i = 0; i < threads; ++i) {
                mThreads.push_back(std::thread(&doWork, this));
            }
        }


        void addTask(Callable const &task) {
            mQueue.push(task);
        }

        ~ThreadPool() {
            while (!mQueue.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(TIME_OUT));
            }
            mFinished.store(true);
            for (auto &thread : mThreads) {
                thread.join();
            }
        }

    private:
        const int TIME_OUT = 100;

        void doWork() {
            while (true) {
                if (mFinished.load()) {
                    return;
                }
                Callable func;
                if (!mQueue.tryPop(TIME_OUT, func)) {
                    continue;
                }
                func();
            }
        }

        std::vector<std::thread> mThreads;
        SyncQueue<Callable> mQueue;
        std::atomic<bool> mFinished;
    };
}


#include "SyncQueue.cpp"

#endif // GRAPHIC_LIBRARY_MULTITHREADIMG_H
