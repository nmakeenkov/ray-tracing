#include "MultiThreading.h"

using namespace MultiThreading;
using namespace std;

ThreadPool::ThreadPool(int threads) : mThreads() {
    mFinished.store(false);
    if (threads < 0) {
        threads = thread::hardware_concurrency();
    }
    threads = max(1, threads);
    for (int i = 0; i < threads; ++i) {
        mThreads.push_back(thread(&doWork, this));
    }
}

void ThreadPool::addTask(std::function<void()> const &task) {
    mQueue.push(task);
}

ThreadPool::~ThreadPool() {
    while (!mQueue.empty()) {
        this_thread::sleep_for(chrono::milliseconds(TIME_OUT));
    }
    mFinished.store(true);
    for (auto &thread : mThreads) {
        thread.join();
    }
}

void ThreadPool::doWork() {
    while (true) {
        if (mFinished.load()) {
            return;
        }
        std::function<void()> func;
        if (!mQueue.tryPop(TIME_OUT, func)) {
            continue;
        }
        func();
    }
}
