/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

#define TIMEOUT_TIME 5000
#define REFILL_TIME 1000

Kitchen::Kitchen(size_t nbCooks, int pid, int refillingTime) : _nbCooks(nbCooks), _pid(pid), _refillingTime(refillingTime)
{
    _InfoBus.b_kitchenWorking = true;

    _InfoBus.b_activeCookers.resize(_nbCooks, false);

    for (size_t i = 0; i < _nbCooks; i++) {
        Cook *cook = new Cook(i, _InfoBus);
        std::thread t = std::thread(&Cook::CookingLoop, cook);
        _cooks.push_back(std::move(t));
    }
    {
        std::unique_lock<std::mutex> lck(_InfoBus.b_coutMutex);
    }
    Timer *counter = new Timer(_InfoBus, TIMEOUT_TIME);
    _timerThread = std::thread(&Timer::Countdown, counter);

    Stock *restock = new Stock(_InfoBus, refillingTime);
    _restockThread = std::thread(&Stock::restock, restock);
}

Kitchen::~Kitchen()
{
    for (auto &cook : _cooks) {
        cook.join();
    }
    _InfoBus.b_kitchenWorking = false;
    _timerThread.join();
    _restockThread.join();
}

bool Kitchen::receiveOrder(int executionTime, plazza::PizzaType orderName)
{
    std::unique_lock<std::mutex> lck(_InfoBus.b_coutMutex);
    Order orderPair(executionTime, orderName);
    if (_InfoBus.b_kitchenWorking && _InfoBus.b_orders.size() < _nbCooks) {
        _InfoBus.b_orders.push(orderPair);
        return true;
    }
    return false;
}

std::vector<Order> Kitchen::getFinishedOrders()
{
    std::vector<Order> orders;
    {
        std::unique_lock<std::mutex> lck(_InfoBus.b_coutMutex);
        while (!_InfoBus.b_finishedOrders.empty()) {
            Order ord;
            _InfoBus.b_finishedOrders.pop(ord);
            orders.push_back(ord);
        }
    }
    return orders;
}

bool Kitchen::isIdle()
{
    for (bool cook : _InfoBus.b_activeCookers) {
        if (cook)
            return false;
        }
    return true;
}

void Kitchen::getStatus()
{
    std::unique_lock<std::mutex> lck(_InfoBus.b_coutMutex);
    std::cout << "================= Kitchen {" << _pid << "} status =================" << std::endl
    << "|\tKitchen working : " << (_InfoBus.b_kitchenWorking ? "{true}" : "{false}") << std::endl
    << "|\tKitchen stocks : {"<< _InfoBus.b_stock.doe << "|" << _InfoBus.b_stock.tomato << "|" << _InfoBus.b_stock.gruyere << "|" << _InfoBus.b_stock.ham << "|" << _InfoBus.b_stock.mushroom << "|" << _InfoBus.b_stock.steak << "|" << _InfoBus.b_stock.eggplant << "|" << _InfoBus.b_stock.goat_cheese << "|" << _InfoBus.b_stock.chief_love << "}" << std::endl
    << "|\tKitchen input queue : {" << _InfoBus.b_orders.size() << "/" << _nbCooks << "}" << std::endl
    << "|\tKitchen active cookers : " << std::endl;
    for (size_t i = 0; i < _InfoBus.b_activeCookers.size(); i++) {
        std::cout << "|\t\tCook " << i << " : " << (_InfoBus.b_activeCookers[i] ? "{true}" : "{false}") << std::endl;
    }
    std::cout << "===========================" << std::string(std::to_string(_pid).length(), '=') << "==========================" << std::endl;

    return;
}

int Kitchen::getPid()
{
    return _pid;
}

bool Kitchen::isWorking()
{
    return _InfoBus.b_kitchenWorking;
}
