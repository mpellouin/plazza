/*
** EPITECH PROJECT, 2022
** plazza
** File description:
** Pizza
*/

#ifndef PIZZA_HPP_
#define PIZZA_HPP_

#include <string>

namespace plazza {
    enum PizzaType {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8
    };

    enum PizzaSize {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    class Pizza {
        public:
            Pizza(plazza::PizzaType name, plazza::PizzaSize size, int number);
            ~Pizza();

            plazza::PizzaType getName() const;
            plazza::PizzaSize getSize() const;
            int getNumber() const;
            void setNumber(int number);

        protected:
        private:
            plazza::PizzaType _name;
            plazza::PizzaSize _size;
            int _number;
    };
}
#endif /* !PIZZA_HPP_ */
