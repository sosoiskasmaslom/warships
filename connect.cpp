#include <iostream>
#include "jack.cpp"


int main() {
    try {
        jack client("127.0.0.1", 8080);

        client << "Привет" << " от клиента";

        std::string reply;
        client >> reply;
        std::cout << "Ответ сервера: " << reply << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}
