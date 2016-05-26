#ifndef GRAPHIC_LIBRARY_SYNCQUEUE_CPP
#define GRAPHIC_LIBRARY_SYNCQUEUE_CPP

#include "MultiThreading.h"

using namespace MultiThreading;

template <typename T>
SyncQueue<T>::SyncQueue() : mFront(NULL), mQueue(), mMutex(), mAbleToPop() { }

template <typename T>
SyncQueue<T>::~SyncQueue() { }

template <typename T>
void SyncQueue<T>::push(const T &e) {
    mMutex.lock();
    mQueue.push(e);
    if (mQueue.size() == 1) {
        mFront = &mQueue.front();
    }
    mMutex.unlock();
    mAbleToPop.notify_one();
}

template <typename T>
T SyncQueue<T>::front() {
    T* ans = mFront.load();
    if (ans == NULL) {
        throw TakingElementFromEmptyQueueException();
    }
    return (*ans);
}

template <typename T>
void SyncQueue<T>::pop() {
    std::unique_lock<std::mutex> uniqueLock(mMutex);
    mAbleToPop.wait(uniqueLock, [this]{ return !mQueue.empty(); });
    mQueue.pop();
    T *newFront = NULL;
    if (!mQueue.empty()) {
        newFront = &mQueue.front();
    }
    mFront.store(newFront);
}

template <typename T>
bool SyncQueue<T>::tryPop(int milliseconds, T &result) {
    std::unique_lock<std::mutex> uniqueLock(mMutex);
    if (!mAbleToPop.wait_for(uniqueLock, std::chrono::milliseconds(milliseconds),
                             [this]{ return !mQueue.empty(); })) {
        return false;
    }
    result = mQueue.front();
    mQueue.pop();
    T *newFront = NULL;
    if (!mQueue.empty()) {
        newFront = &mQueue.front();
    }
    mFront.store(newFront);
}

template <typename T>
std::size_t SyncQueue<T>::size() {
    std::unique_lock<std::mutex> uniqueLock(mMutex);
    return mQueue.size();
}

template <typename T>
bool SyncQueue<T>::empty() {
    return (size() == 0);
}

template<typename T>
const char* SyncQueue<T>::TakingElementFromEmptyQueueException::what() const throw() {
    return "Attempt to call front() in empty queue";
}


#endif // GRAPHIC_LIBRARY_SYNCQUEUE_CPP
