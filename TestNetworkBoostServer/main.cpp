#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using boost::asio::ip::tcp;

int main() {
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);  

    try {
        // Ініціалізація контексту вводу/виводу
        boost::asio::io_context io_context;

        // Створення TCP acceptor для прослуховування підключень на порту 12345
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Сервер запущений та очікує на підключення..." << std::endl;

        // Очікування клієнта
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Клієнт підключився!" << std::endl;

        // Відкриваємо файл для читання
        std::ifstream file("response.txt", std::ios::binary);
        if( !file.is_open() ) {
            std::cerr << "Помилка: не вдається відкрити файл response.txt" << std::endl;
            return 1;
        }

        // Читаємо вміст файлу
        std::string file_content( (std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>() );

        // Відправляємо файл через сокет
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(file_content), error);

        if(error) std::cerr << "Помилка при відправленні файлу: " << error.message() << std::endl;
        else std::cout << "Файл успішно відправлено клієнту!" << std::endl;

        // Закриваємо з'єднання
        socket.shutdown(tcp::socket::shutdown_both, error);
        socket.close();
        std::cout << "З'єднання закрито." << std::endl;
    } catch(std::exception& e) {
        std::cerr << "Виняток: " << e.what() << std::endl;
    }

    return 0;
}
