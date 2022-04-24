#include "share_sock.h"

Share_sock::Share_sock(tcp::socket socket, chat_room &room, Database &db) : socket_(std::move(socket)), room_(room), db_(db)
{
    std::cout << &room_ << std::endl;
    std::cout << &db_ << std::endl;
}

    void Share_sock::start()
    {
        boost::asio::write(socket_, boost::asio::buffer("heel", 4));
        room_.join(shared_from_this());
        do_read_baotou();
    }