/*
** EPITECH PROJECT, 2022
** plazza_test
** File description:
** main
*/

#include <map>
#include "Shell.hpp"
#include "Kitchen.hpp"

int main (int ac, char **av)
{
    if ((ac == 2 && av[1] == std::string("--help")) || ac != 4) {
        (ac == 2 ? std::cout : std::cerr) << "USAGE: ./plazza [cook_time] [cook_count] [restock_time]"
            << std::endl;
        return (ac == 2 ? 0 : 84);
    }
    Shell shell(0, 0, 0);
    remove("log.txt");
    try {
        shell = Shell(std::stoi(av[1]), std::stoi(av[2]), std::stoi(av[3]));
        while (true)
            shell.process();
    } catch(const std::exception &err) {
        std::cerr << err.what() << std::endl;
        shell.closeKitchens();
    }
    return 0;
}
