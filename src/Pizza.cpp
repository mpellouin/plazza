/*
** EPITECH PROJECT, 2022
** plazza
** File description:
** Pizza
*/

#include "Pizza.hpp"

plazza::Pizza::Pizza(plazza::PizzaType name, plazza::PizzaSize size, int number) : _name {name}, _size {size}, _number {number}
{
}

plazza::Pizza::~Pizza()
{
}

plazza::PizzaType plazza::Pizza::getName() const
{
    return _name;
}

plazza::PizzaSize plazza::Pizza::getSize() const
{
    return _size;
}

int plazza::Pizza::getNumber() const
{
    return _number;
}

void plazza::Pizza::setNumber(int number)
{
    _number = number;
}