/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** Timer
*/

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "InformationBus.hpp"

class Timer {
    public:
        Timer(InformationBus &infoBus, size_t delay) : _LocalBus(infoBus), _delay(delay){};

        bool isIdle() {
            std::unique_lock<std::mutex> lck(_LocalBus.b_coutMutex);
            for (bool cook : _LocalBus.b_activeCookers) {
                if (cook)
                    return false;
            }
            return true;
        }

        void Countdown() {
            _time = std::chrono::system_clock::now();

            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time).count() < std::chrono::milliseconds(_delay).count()) {
                if (!isIdle()) {
                    Reset();
                } else {
                    {
                        std::unique_lock<std::mutex> lck(_LocalBus.b_coutMutex);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            _LocalBus.b_kitchenWorking = false;
        }

        void Reset() {
            _time = std::chrono::system_clock::now();
        }

        ~Timer();

    private:
        std::chrono::system_clock::time_point _time;
        InformationBus &_LocalBus;
        size_t _delay;
};

#endif /* !TIMER_HPP_ */
