// #include "share_sock.h"
#include <boost/asio.hpp>
#include "sock_server.h"

#include <boost/json/src.hpp>
#include <boost/json/object.hpp>
#include <boost/json.hpp>
#include <string>

#include <iostream>
using boost::asio::ip::tcp;
// 消息队列
// 抽象基类的智能指
int main()
{
    boost::asio::io_service io_service;
    tcp::endpoint ep(boost::asio::ip::tcp::v4(), 9999);

    sock_server cs(io_service, ep);
    try
    {
        io_service.run();
    }
    catch (std::exception &ex)
    {
        std::cout << "Exception:" << ex.what() << std::endl;
        return 0;
    }
}