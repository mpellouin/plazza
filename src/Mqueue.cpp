/*
** EPITECH PROJECT, 2022
** plazza
** File description:
** Mqueue
*/

#include "Mqueue.hpp"

Mqueue::Mqueue(std::string name, int flags)
{
    struct mq_attr mqAttr;
    mqAttr.mq_maxmsg = 10;
    mqAttr.mq_msgsize = 20;
    _mq = mq_open(name.c_str(), flags, PMODE, &mqAttr);
    if (_mq == (mqd_t) -1)
        throw std::runtime_error("mq_open failed");
}

Mqueue::~Mqueue()
{
    mq_close(_mq);
}

int Mqueue::send(std::string msg)
{
    if (mq_send(_mq, msg.c_str(), msg.length(), 0) == -1 && errno != EAGAIN)
        throw std::runtime_error("mq_send failed");
    return (0);
}

std::string Mqueue::receive()
{
    char buffer[20];
    memset(buffer, 0, 20);
    if (mq_receive(_mq, buffer, 20, NULL) == -1 && errno != EAGAIN)
        throw std::runtime_error("mq_receive failed");
    return (std::string(buffer));
}

void Mqueue::unlink(std::string name)
{
    if (mq_unlink(name.c_str()) == -1)
        throw std::runtime_error("mq_unlink failed " + std::string(strerror(errno)) + " " + name);
}