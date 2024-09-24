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
        // ����������� ��������� �����/������
        boost::asio::io_context io_context;

        // ��������� TCP acceptor ��� ��������������� ��������� �� ����� 12345
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "������ ��������� �� ����� �� ����������..." << std::endl;

        // ���������� �볺���
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "�볺�� ����������!" << std::endl;

        // ³�������� ���� ��� �������
        std::ifstream file("response.txt", std::ios::binary);
        if( !file.is_open() ) {
            std::cerr << "�������: �� ������� ������� ���� response.txt" << std::endl;
            return 1;
        }

        // ������ ���� �����
        std::string file_content( (std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>() );

        // ³���������� ���� ����� �����
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(file_content), error);

        if(error) std::cerr << "������� ��� ���������� �����: " << error.message() << std::endl;
        else std::cout << "���� ������ ���������� �볺���!" << std::endl;

        // ��������� �'�������
        socket.shutdown(tcp::socket::shutdown_both, error);
        socket.close();
        std::cout << "�'������� �������." << std::endl;
    } catch(std::exception& e) {
        std::cerr << "�������: " << e.what() << std::endl;
    }

    return 0;
}
