#include <iostream>
#include "jack.cpp" 


int main() {
    try {
        jackhost server(8080);
        std::cout << "Ожидаем клиента..." << std::endl;

        server.wait_jack();
        std::cout << "Клиент подключён!" << std::endl;

        server << "Привет от сервера!";

        std::string msg;
        server >> msg;
        std::cout << "Клиент прислал: " << msg << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}
