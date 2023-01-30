/*
** EPITECH PROJECT, 2022
** plazza
** File description:
** Shell
*/

#include "Shell.hpp"

Shell::Shell(double cookingTime, int nbCooks, int restockTime) : _cookingTime {cookingTime}, _nbCooks {nbCooks}, _restockTime {restockTime}
{
    _main = getpid();
}

Shell::~Shell()
{
}

void Shell::readPizzas()
{
    std::string line, token;
    std::string delimiter = ";";
    struct timeval tv;
    fd_set fd;

    FD_ZERO(&fd);
    FD_SET(0, &fd);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int somethingToRead = select(1, &fd, NULL, NULL, &tv);

    if (somethingToRead == -1)
        throw std::runtime_error("Error: select failed");
    if (somethingToRead == 0) return;
    getline(std::cin, line);
    if (line.empty()) return;
    if (line == "status") {_statusRequested = true;return;}
    while (line.length() > 0) {
        std::string pizzaName, pizzaSize, pizzaNumber;
        std::stringstream ss;
        if (line.find(delimiter) == std::string::npos) {
            token = line;
            line = "";
        } else {
            token = line.substr(0, line.find(delimiter));
            line = line.substr(line.find(delimiter) + 1);
        }
        ss.str(token);
        ss >> pizzaName >> pizzaSize >> pizzaNumber;
        _addPizza(pizzaName, pizzaSize, pizzaNumber);
    }
}

void Shell::_addPizza(std::string pizzaName, std::string pizzaSize, std::string pizzaNumber)
{
    plazza::PizzaSize size;
    plazza::PizzaType type;
    int number;

    if (pizzaSize == "S")
        size = plazza::S;
    else if (pizzaSize == "M")
        size = plazza::M;
    else if (pizzaSize == "L")
        size = plazza::L;
    else if (pizzaSize == "XL")
        size = plazza::XL;
    else if (pizzaSize == "XXL")
        size = plazza::XXL;
    else
        throw PizzaException("Invalid size");
    
    if (pizzaName == "Regina")
        type = plazza::Regina;
    else if (pizzaName == "Margarita")
        type = plazza::Margarita;
    else if (pizzaName == "Americana")
        type = plazza::Americana;
    else if (pizzaName == "Fantasia")
        type = plazza::Fantasia;
    else
        throw PizzaException("Invalid name");
    
    if (pizzaNumber.length() < 2 || pizzaNumber.find("x") != 0)
        throw PizzaException("Invalid number");
    number = std::stoi(pizzaNumber.substr(1));

    _pizzas.push(plazza::Pizza(type, size, number));
}

void Shell::dispersePizzas()
{
    pid_t pid;

    if (_statusRequested) _askForStatus();
    for (_reduced = false; _pizzas.size() > 0; _reduced = false) {
        for (auto &kitchen : _kitchens) {
            _assignPizzas(kitchen);
            if (_pizzas.size() == 0)
                break;
        }
        if (!_reduced && _pizzas.front().getNumber() > 0) {
            pid = fork();
            _kitchens.push_back(pid);
            if (pid > 0) {
                _assignPizzas(pid);
            } else return;
        }
    }
}

void Shell::readmq(Kitchen *kitchen)
{
    std::string pizzaName, pizzaSize, sBuffer;
    int cookingTime;
    Mqueue mq("/s" + std::to_string(getpid()), O_RDONLY|O_NONBLOCK);
    sBuffer = mq.receive();
    if (sBuffer.empty()) return;
    if (sBuffer == "STATUS") {
        kitchen->getStatus();
        Mqueue mq("/r" + std::to_string(getpid()), O_WRONLY|O_NONBLOCK);
        mq.send("STATUS");
        return;
    }
    try {
        pizzaName = sBuffer.substr(0, sBuffer.find(";"));
        pizzaSize = sBuffer.substr(sBuffer.find(";") + 1);
    } catch (std::exception &e) {
        throw Shell::PizzaException("Couldn't read message");
    }
    cookingTime = _computeCookingTime(pizzaName);

    Mqueue response("/r" + std::to_string(getpid()), O_WRONLY|O_CREAT);
    response.send(kitchen->receiveOrder(cookingTime, _getPizzaId(pizzaName)) ? "OK" : "KO");
    return;
}

void Shell::closeKitchens()
{
    for (auto &kitchen : _kitchens) {
        kill(kitchen, SIGKILL);
    }
}

struct mq_attr Shell::getmqattr()
{
    struct mq_attr mqAttr;
    mqAttr.mq_maxmsg = 10;
    mqAttr.mq_msgsize = 20;
    return mqAttr;
}

void Shell::_assignPizzas(int pid)
{
    Mqueue mq("/s" + std::to_string(pid), O_WRONLY|O_CREAT);
    const char *msg = (std::to_string((int)_pizzas.front().getName()) + ";" + std::to_string((int)_pizzas.front().getSize())).c_str();
    
    mq.send(msg);

    Mqueue response("/r" + std::to_string(pid), O_RDONLY|O_CREAT);
    std::string sBuffer = response.receive();
    
    if (sBuffer == "OK") {
        _pizzas.front().setNumber(_pizzas.front().getNumber() - 1);
        _reduced = true;
    }
    if (_pizzas.front().getNumber() == 0) _pizzas.pop();
}

int Shell::_computeCookingTime(std::string pizzaName)
{
    plazza::PizzaType pizza = _getPizzaId(pizzaName);
    double cookingTime = 0;

    switch (pizza) {
        case plazza::Regina:
            cookingTime = 1000 * _cookingTime;
            break;
        case plazza::Margarita:
            cookingTime = 2000 * _cookingTime;
            break;
        case plazza::Americana:
            cookingTime = 2000 * _cookingTime;
            break;
        case plazza::Fantasia:
            cookingTime = 4000 * _cookingTime;
            break;
        default:
            throw PizzaException("Invalid pizza");
    }
    return cookingTime;
}

plazza::PizzaType Shell::_getPizzaId(std::string pizzaName)
{
    if (pizzaName == "1")
        return plazza::Regina;
    else if (pizzaName == "2")
        return plazza::Margarita;
    else if (pizzaName == "4")
        return plazza::Americana;
    else if (pizzaName == "8")
        return plazza::Fantasia;
    else
        throw PizzaException("Invalid name");
}

void Shell::checkKitchens()
{
    for (auto &kitchen : _kitchens) {
        Mqueue mq("/r" + std::to_string(kitchen), O_CREAT|O_RDONLY|O_NONBLOCK);
        std::string sBuffer = mq.receive();
        if (sBuffer == "OVER") {
            Mqueue::unlink("/r" + std::to_string(kitchen));
            waitpid(kitchen, NULL, 0);
            _kitchens.erase(_kitchens.begin() + std::distance(_kitchens.begin(), std::find(_kitchens.begin(), _kitchens.end(), kitchen)));
        }
        if (sBuffer.substr(0, 4) == "DONE") {
            std::string pizzaName = sBuffer.substr(5);
            pizzaName = pizzaName == "1" ? "Regina" : pizzaName == "2" ? "Margarita" : pizzaName == "4" ? "Americana" : pizzaName == "8" ? "Fantasia" : "";
            std::cout << "A pizza " << pizzaName << " has been cooked by kitchen " << kitchen << std::endl;
            std::ofstream file("log.txt", std::ios::app);
            char buffer[30];
            memset(buffer, 0, 30);
            time_t now = time(NULL);
            strftime(buffer, 30, "%d-%m-%Y %Hh%M %Ss", localtime(&now));
            file << buffer << ": kitchen " << kitchen << " has finished cooking " << pizzaName << std::endl;
        }
    }
}

void Shell::closeKitchen(int pid)
{
    Mqueue mq("/r" + std::to_string(pid), O_WRONLY|O_NONBLOCK);
    mq.send("OVER");
    Mqueue::unlink("/s" + std::to_string(pid));
}

void Shell::announcePizza(Kitchen *kitchen)
{
    if (kitchen == NULL)
        return;
    std::vector<Order> pizza = kitchen->getFinishedOrders();
    if (pizza.empty())
        return;
    Mqueue mq("/r" + std::to_string(kitchen->getPid()), O_WRONLY|O_NONBLOCK);
    while (!pizza.empty()) {
        mq.send("DONE " + std::to_string(pizza[0].second));
        pizza.erase(pizza.begin());
    }
}

void Shell::_askForStatus()
{
    for (auto &kitchen : _kitchens) {
        Mqueue mq("/s" + std::to_string(kitchen), O_WRONLY|O_NONBLOCK);
        mq.send("STATUS");
        Mqueue response("/r" + std::to_string(kitchen), O_RDONLY);
        std::string sBuffer = response.receive();
    }
    _statusRequested = false;
}

void Shell::process()
{
    if (getpid() == _main) {
        readPizzas();
        checkKitchens();
        dispersePizzas();
    } else {
        if (_kitchen == NULL) {
            _kitchen = new Kitchen(_nbCooks, getpid(), _restockTime);
        }
        if (!_kitchen->isWorking()) {
            delete _kitchen;
            closeKitchen(getpid());
            return ;
        }
        readmq(_kitchen);
        announcePizza(_kitchen);
    }
}