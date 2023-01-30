/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** SafeQueue
*/

#ifndef SAFEQUEUE_HPP_
#define SAFEQUEUE_HPP_

#include <mutex>
#include <queue>
#include <iostream>
#include <condition_variable>


template <typename T>
class SafeQueue {
    public:
        explicit SafeQueue(bool blocking = true) : _locker(blocking) {};
        ~SafeQueue() = default;

        void push(const T& item) {
            std::scoped_lock lock(_mutex);
            _queue.push(item);
            _cond.notify_one();
        }
        void push(T&& item) {
            std::scoped_lock lock(_mutex);
            _queue.push(std::move(item));
            _cond.notify_one();
        }
        bool try_push(const T& item) {
            std::unique_lock lock(_mutex, std::try_to_lock);
            if (!lock)
                return false;
            _queue.push(item);
            _cond.notify_one();
            return true;
        }
        bool try_push(T&& item) {
            std::unique_lock lock(_mutex, std::try_to_lock);
            if (!lock)
                return false;
            _queue.push(std::move(item));
            _cond.notify_one();
            return true;
        }
        bool pop(T& item) {
            std::unique_lock lock(_mutex);
            _cond.wait(lock, [this]{ return !_queue.empty() || !_locker; });
            if (_queue.empty())
                return false;
            item = std::move(_queue.front());
            _queue.pop();
            return true;
        }
        bool try_pop(T& item) {
            std::unique_lock lock(_mutex, std::try_to_lock);
            if (!lock || _queue.empty())
                return false;
            item = std::move(_queue.front());
            _queue.pop();
            return true;
        }
        size_t size() const {
            std::scoped_lock lock(_mutex);
            return _queue.size();
        }
        bool empty() {
            std::scoped_lock lock(_mutex);
            return _queue.empty();
        }
        void block() {
            std::scoped_lock lock(_mutex);
            _locker = true;
        }
        void unblock() {
            std::scoped_lock lock(_mutex);
            _locker = false;
            _cond.notify_all();
        }
        bool blocked() const {
            std::scoped_lock lock(_mutex);
            return _locker;
        }


    private:
        std::queue<T> _queue;
        bool _locker;
        mutable std::mutex _mutex;
        std::condition_variable _cond;
};

#endif /* !SAFEQUEUE_HPP_ */
