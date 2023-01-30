/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** Cook
*/

#include "Cook.hpp"

Cook::Cook(size_t id, InformationBus &infoBus) : _id(id), _LocalBus(infoBus)
{
    std::unique_lock<std::mutex> lock(_LocalBus.b_coutMutex);
    _LocalBus.b_activeCookers[_id] = false;
}

Cook::~Cook()
{
}

bool Cook::isAvailable()
{
    return _LocalBus.b_activeCookers[_id];
}

std::chrono::milliseconds Cook::cookingTimeElapsed()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _startTime);
}

void Cook::cookingTime()
{
    if (cookingTimeElapsed() > std::chrono::milliseconds(_currentOrder.first)) {
        _LocalBus.b_finishedOrders.push(_currentOrder);
        _LocalBus.b_activeCookers[_id] = false;
    }
}

bool Cook::confirmIngredients(plazza::PizzaType pizza)
{
    if (pizza == plazza::PizzaType::Margarita) {
        if (_LocalBus.b_stock.doe > 0 && _LocalBus.b_stock.tomato > 0 && _LocalBus.b_stock.gruyere > 0) {
            _LocalBus.b_stock.doe--;
            _LocalBus.b_stock.tomato--;
            _LocalBus.b_stock.gruyere--;
            return true;
        }
    } else if (pizza == plazza::PizzaType::Regina) {
        if (_LocalBus.b_stock.doe > 0 && _LocalBus.b_stock.tomato > 0 && _LocalBus.b_stock.gruyere > 0 && _LocalBus.b_stock.ham > 0 && _LocalBus.b_stock.mushroom > 0) {
            _LocalBus.b_stock.doe--;
            _LocalBus.b_stock.tomato--;
            _LocalBus.b_stock.gruyere--;
            _LocalBus.b_stock.ham--;
            _LocalBus.b_stock.mushroom--;
            return true;
        }
    } else if (pizza == plazza::PizzaType::Americana) {
        if (_LocalBus.b_stock.doe > 0 && _LocalBus.b_stock.tomato > 0 && _LocalBus.b_stock.gruyere > 0 && _LocalBus.b_stock.steak > 0) {
            _LocalBus.b_stock.doe--;
            _LocalBus.b_stock.tomato--;
            _LocalBus.b_stock.gruyere--;
            _LocalBus.b_stock.steak--;
            return true;
        }
    } else if (pizza == plazza::PizzaType::Fantasia) {
        if (_LocalBus.b_stock.doe > 0 && _LocalBus.b_stock.tomato > 0 && _LocalBus.b_stock.eggplant > 0 && _LocalBus.b_stock.goat_cheese > 0 && _LocalBus.b_stock.chief_love > 0) {
            _LocalBus.b_stock.doe--;
            _LocalBus.b_stock.tomato--;
            _LocalBus.b_stock.eggplant--;
            _LocalBus.b_stock.goat_cheese--;
            _LocalBus.b_stock.chief_love--;
            return true;
        }
    }
    return false;
}

void Cook::setOrder(Order ord)
{
    if (confirmIngredients(ord.second)) {
        _LocalBus.b_activeCookers[_id] = true;
        _currentOrder = ord;
        _startTime = std::chrono::system_clock::now();
    } else {
        _LocalBus.b_orders.push(ord);
    }
}

void Cook::CookingLoop()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!_LocalBus.b_kitchenWorking && _LocalBus.b_orders.empty()) break;
        {
            std::unique_lock<std::mutex> lck(_LocalBus.b_coutMutex);
            if (!isAvailable()) {
                Order ord;
                if (_LocalBus.b_orders.try_pop(ord)) {
                    setOrder(ord);
                }
            } else {
                cookingTime();
            }
        }
    }
}

// Regina XL x2; Margarita XL x1; Americana XL x1; Fantasia XL x1;
