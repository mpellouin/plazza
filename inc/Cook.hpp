/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** Cook
*/

#ifndef COOK_HPP_
#define COOK_HPP_

#include "InformationBus.hpp"

class Cook {
    public:
        Cook(size_t id, InformationBus &infoBus);
        ~Cook();

        bool isAvailable();
        std::chrono::milliseconds cookingTimeElapsed();
        void cookingTime();
        bool confirmIngredients(plazza::PizzaType pizza);
        void setOrder(Order ord);
        void CookingLoop();

    private:
        size_t _id;
        Order _currentOrder;

        std::chrono::system_clock::time_point _startTime;

        InformationBus &_LocalBus;
};

#endif /* !COOK_HPP_ */
