#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

void read_thread(tcp::socket& socket) {
    try {
        while (true) {
            boost::asio::streambuf buf;
            boost::asio::read_until(socket, buf, "\n");
            std::string message = boost::asio::buffer_cast<const char*>(buf.data());
            std::cout << "Received: " << message << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Read thread: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));

        std::thread reader_thread(read_thread, std::ref(socket));

        while (true) {
            std::string message;
            std::getline(std::cin, message);
            boost::asio::write(socket, boost::asio::buffer(message + "\n"));
        }

        reader_thread.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}