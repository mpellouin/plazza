/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** InformationBus
*/

#ifndef INFORMATIONBUS_HPP_
#define INFORMATIONBUS_HPP_

#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <memory>
#include <iostream>
#include <functional>
#include "SafeQueue.hpp"
#include "Pizza.hpp"

using Order = std::pair<int, plazza::PizzaType>;

struct Storage {
    size_t doe;
    size_t tomato;
    size_t gruyere;
    size_t ham;
    size_t mushroom;
    size_t steak;
    size_t eggplant;
    size_t goat_cheese;
    size_t chief_love;
};

struct InformationBus {
    SafeQueue<Order> b_orders;
    SafeQueue<Order> b_finishedOrders;
    std::mutex b_coutMutex;
    std::vector<bool> b_activeCookers;
    bool b_kitchenWorking;
    Storage b_stock;
};

#endif /* !INFORMATIONBUS_HPP_ */
