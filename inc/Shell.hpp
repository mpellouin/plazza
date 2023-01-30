/*
** EPITECH PROJECT, 2022
** plazza
** File description:
** Shell
*/

#ifndef SHELL_HPP_
#define SHELL_HPP_

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <vector>
#include <errno.h>
#include <iterator>
#include <algorithm>
#include "Pizza.hpp"
#include "Kitchen.hpp"
#include "Mqueue.hpp"

class Shell {
    public:
        Shell(double cookingTime, int nbCooks, int restockTime);
        ~Shell();

        void readPizzas();
        void dispersePizzas();
        void readmq(Kitchen *kitchen);
        void announcePizza(Kitchen *kitchen);
        void closeKitchens();
        void checkKitchens();
        void closeKitchen(int pid);
        static struct mq_attr getmqattr();
        void process();

        class PizzaException : public std::exception {
            public:
                PizzaException(std::string message) : _message(message) {}
                ~PizzaException() {}
                const char *what() const throw() { return _message.c_str(); }
            private:
                std::string _message;
        };

    protected:
    private:
        double _cookingTime;
        int _nbCooks;
        int _restockTime;
        std::queue<plazza::Pizza> _pizzas;
        std::vector<pid_t> _kitchens;
        struct mq_attr _mqAttr = {getmqattr()};
        bool _reduced = false;
        bool _statusRequested = false;

        void _addPizza(std::string pizzaName, std::string pizzaSize, std::string pizzaNumber);
        void _assignPizzas(int pid);
        int _computeCookingTime(std::string pizzaSize);
        plazza::PizzaType _getPizzaId(std::string pizzaName);
        void _askForStatus();
        int _main;
        Kitchen *_kitchen = NULL;
};

#endif /* !SHELL_HPP_ */
