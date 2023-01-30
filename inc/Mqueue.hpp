/*
** EPITECH PROJECT, 2022
** plazza
** File description:
** Mqueue
*/

#ifndef MQUEUE_HPP_
#define MQUEUE_HPP_

#include <mqueue.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <string.h>

#define PMODE 0655

class Mqueue {
    public:
        Mqueue(std::string name, int flags);
        ~Mqueue();

        int send(std::string msg);
        std::string receive();
        static void unlink(std::string name);
    protected:
    private:
        mqd_t _mq;
};

#endif /* !MQUEUE_HPP_ */
