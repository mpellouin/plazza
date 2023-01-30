/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** Kitchen
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_


#include "Cook.hpp"
#include "Timer.hpp"
#include "Stock.hpp"
#include "SafeQueue.hpp"
#include "InformationBus.hpp"
#include "Pizza.hpp"

class Kitchen {
    public:
        Kitchen(size_t nbCooks, int pid, int refillingTime);
        ~Kitchen();

        bool receiveOrder(int executionTime, plazza::PizzaType order);
        void processingLoop();
        int getPid();
        bool isWorking();
        std::vector<Order> getFinishedOrders();
        bool isIdle();
        void getStatus();

    private:
        std::thread _timerThread;
        std::thread _restockThread;
        std::vector<std::thread> _cooks;
        size_t _nbCooks;
        int _pid;
        int _refillingTime;

        InformationBus _InfoBus;

};

#endif /* !KITCHEN_HPP_ */
