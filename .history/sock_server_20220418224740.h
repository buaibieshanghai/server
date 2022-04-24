#include <boost/asio.hpp>
using boost::asio::ip::tcp;
class sock_server
{
public:
     sock_server(boost::asio::io_service &io_service, const tcp::endpoint &endpoint) : acceptor_(io_service, endpoint), socket_(io_service)
    {
        do_accept();

        std::cout << &db_ << std::endl;
        std::cout << &room_ << std::endl;
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec)
                               {
        if(!ec){
            // 有客户端连接 创建一个socket的智能指针 并且初始化加入聊天室

            auto session=
            std::make_shared<chat_session>(std::move(socket_),room_,db_);
std::cout <<session<<std::endl;
            session->start();

std::cout <<"有新客户端连接"<<std::endl;
        }
        do_accept(); });
    }

private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    Sock_room room_;
    Database db_;
};
