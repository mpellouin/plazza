/*
** EPITECH PROJECT, 2022
** plazza_test
** File description:
** main
*/

#include <map>

#include "SafeQueue.hpp"
#include "Kitchen.hpp"

const size_t NB_THREADS = 5;

std::vector<std::pair<size_t, std::string>> generateOrders(size_t numberOfPizza)
{
    std::map<size_t, std::string> pizzaName = {
        {1, "Margarita"},
        {2, "Reine"},
        {3, "Napolitaie"},
        {4, "Hawaienne"},
        {5, "Cannibale"},
        {6, "4 fromages"},
        {7, "4 Jambons"},
        {8, "Vegetarienne"},
        {9, "Savoyarde"},
        {10, "Calzonne"}
    };

    std::vector<std::pair<size_t, std::string>> orders;
    for (size_t i = 0; i < numberOfPizza; ++i) {
        orders.push_back(std::make_pair(rand() % 500, std::to_string(i) + " - " + pizzaName[rand() % 10 + 1]));
    }
    return orders;
}


int main (void)
{
    srand(time(NULL));

    std::vector<std::pair<size_t, std::string>> future_orders = generateOrders(15);
    Kitchen kitchen(NB_THREADS, 100, 1000);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    for (size_t i = 0; i < future_orders.size(); ++i) {
       bool res = kitchen.receiveOrder(future_orders[i].first, plazza::PizzaType::Regina);
        if (!res) {
            i--;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    kitchen.getStatus();
}
