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

    void Share_sock::deliver(boost::json::object var)
    {

        std::cout << var << std::endl;
        boost::json::serializer ser;
        ser.reset(&var);
        while (!ser.done())
        {
            //     // 具有推断的
            char baoti[510];
            int index = ser.read(baoti).size();
            sprintf(data, "%4d%.*s", index, 510, baoti);
            do_write(index + 4);
        }
    }