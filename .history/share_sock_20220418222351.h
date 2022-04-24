
#ifndef __SHARESOCK__h
#define __SHARESOCK__h
#include <mysql.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/object.hpp>
#include <boost/json/value.hpp>
#include <boost/json/string.hpp>
#include <boost/format.hpp>
#include <boost/json.hpp>
#include <boost/format/free_funcs.hpp>
#include <string>
#endif
class Share_sock : public std::enable_shared_from_this<Share_sock>
{
public:
    Share_sock(tcp::socket socket, chat_room &room, Database &db);
    void start();

    void deliver(boost::json::object var);

private:
    // 对客户端发送的业务处理
    void action(size_t len);

    void
    do_read_baotou()
    {
        //读取包头
        auto self(shared_from_this());
        boost::asio::async_read(
            socket_, boost::asio::buffer(data, 4),
            [this, self](boost::system::error_code ec, std::size_t len)
            {
                std::cout << data << std::endl;
                if (ec)
                {
                    std::cout << "客户端断开" << std::endl;
                    room_.leave(shared_from_this());
                    std::cout << "已释放客户端资源" << std::endl;
                }
                else
                {

                    do_read_baoti(atoi(data));
                } // read事件触发回调函数
            });
    }
    // 读取胞体
    void do_read_baoti(size_t len)
    {
        auto self(shared_from_this());
        boost::asio::async_read(
            socket_, boost::asio::buffer(data, len),
            [this, self](boost::system::error_code ec, std::size_t len)
            {
                // read事件触发回调函数
                if (ec)
                {
                    std::cout << "客户端断开" << std::endl;
                    room_.leave(shared_from_this());
                    std::cout << "已释放客户端资源" << std::endl;
                }
                else
                {
                    // 经过测试 boost的异步read和write事件  会在回调函数结束之后才会触发
                    action(len);
                    do_read_baotou();
                }
            });
    }
    //像客户端发送数据
    void do_write(size_t len)
    {
        auto self(shared_from_this());
        boost::asio::async_write(

            socket_, boost::asio::buffer(data, len),
            [this, self](boost::system::error_code ec, std::size_t len)
            {
                // read事件触发回调函数
                if (ec)
                {
                    std::cout << "客户端断开" << std::endl;
                    room_.leave(shared_from_this());
                    std::cout << "已释放客户端资源" << std::endl;
                }
            });
    }

private:
    tcp::socket socket_;
    chat_room &room_;
    char data[512];

    Database &db_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_service &io_service, const tcp::endpoint &endpoint) : acceptor_(io_service, endpoint), socket_(io_service)
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
    chat_room room_;
    Database db_;
};
