
#pragma once
#include <stdexcept>
#include <string>

// класс для завершения игры.
// по сути класс исключения.
// когда будет деконструироваться класс sTunnel, он вкинет BUE.
// а с той стороны эта ошибка уже выкинет из игры.
// \param msg имя игрока-победителя
class EndGame: public std::runtime_error
{
public:
    explicit EndGame(const std::string& msg):
        std::runtime_error(msg) {}
};