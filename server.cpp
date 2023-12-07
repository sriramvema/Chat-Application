#include <boost/asio.hpp>
#include <iostream>
#include <set>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
using boost::asio::ip::tcp;

class ChatSession;

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context, int port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
        mongocxx::client conn{mongocxx::uri{"mongodb://localhost:27017"}};
        mongocxx::database db = conn["chat_db"];
    }

private:
    void start_accept() {
        auto session = std::make_shared<ChatSession>(acceptor_.get_executor().context());
        acceptor_.async_accept(session->socket(), 
            [this, session](boost::system::error_code ec) {
                if (!ec) {
                    session->start();
                }
                start_accept();
            });
    }

    tcp::acceptor acceptor_;
};

class ChatSession : public std::enable_shared_from_this<ChatSession> {
public:
    ChatSession(boost::asio::io_context& io_context)
        : socket_(io_context) {
    }

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        // Implement message reading from client and broadcasting to other clients
    }

private:
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        int port = 1234;
        ChatServer server(io_context, port);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}