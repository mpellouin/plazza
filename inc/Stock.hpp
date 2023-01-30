/*
** EPITECH PROJECT, 2022
** B-CCP-400-LYN-4-1-theplazza-maxence.pellouin
** File description:
** Stock
*/

#ifndef STOCK_HPP_
#define STOCK_HPP_

#include "InformationBus.hpp"

#define STOCK_LIMIT 5

class Stock {
    public:
        Stock(InformationBus &infoBus, size_t delay) : _LocalBus(infoBus), _delay(delay) {
            _LocalBus.b_stock.doe = 5;
            _LocalBus.b_stock.tomato = 5;
            _LocalBus.b_stock.gruyere = 5;
            _LocalBus.b_stock.ham = 5;
            _LocalBus.b_stock.mushroom = 5;
            _LocalBus.b_stock.steak = 5;
            _LocalBus.b_stock.eggplant = 5;
            _LocalBus.b_stock.goat_cheese = 5;
            _LocalBus.b_stock.chief_love = 5;
            _time = std::chrono::system_clock::now();
        };
        ~Stock();

        void restock() {
            while (_LocalBus.b_kitchenWorking) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time).count() > std::chrono::milliseconds(_delay).count()) {
                    _time = std::chrono::system_clock::now();
                    _LocalBus.b_stock.doe + 1 < STOCK_LIMIT ? _LocalBus.b_stock.doe++ : 0;
                    _LocalBus.b_stock.tomato + 1 < STOCK_LIMIT ? _LocalBus.b_stock.tomato++ : 0;
                    _LocalBus.b_stock.gruyere + 1 < STOCK_LIMIT ? _LocalBus.b_stock.gruyere++ : 0;
                    _LocalBus.b_stock.ham + 1 < STOCK_LIMIT ? _LocalBus.b_stock.ham++ : 0;
                    _LocalBus.b_stock.mushroom + 1 < STOCK_LIMIT ? _LocalBus.b_stock.mushroom++ : 0;
                    _LocalBus.b_stock.steak + 1 < STOCK_LIMIT ? _LocalBus.b_stock.steak++ : 0;
                    _LocalBus.b_stock.eggplant + 1 < STOCK_LIMIT ? _LocalBus.b_stock.eggplant++ : 0;
                    _LocalBus.b_stock.goat_cheese + 1 < STOCK_LIMIT ? _LocalBus.b_stock.goat_cheese++ : 0;
                    _LocalBus.b_stock.chief_love + 1 < STOCK_LIMIT ? _LocalBus.b_stock.chief_love++ : 0;
                }
            }
        }
    private:
        std::chrono::system_clock::time_point _time;
        InformationBus &_LocalBus;
        size_t _delay;
};

#endif /* !STOCK_HPP_ */
