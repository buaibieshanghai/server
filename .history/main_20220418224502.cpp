#include "database.h"
#include "share_sock.h"
#include <boost/asio.hpp>
#include <deque>
#include <list>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <boost/system/system_error.hpp>
#include <cstdlib>

#include <boost/json/src.hpp>

#include <boost/thread.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/object.hpp>
#include <boost/json/value.hpp>
#include <boost/json/string.hpp>
#include <boost/format.hpp>
#include <boost/json.hpp>
#include <boost/format/free_funcs.hpp>
#include <string>

using boost::asio::ip::tcp;
// 消息队列
// 抽象基类的智能指
int main()
{
    boost::asio::io_service io_service;
    tcp::endpoint ep(boost::asio::ip::tcp::v4(), 9999);

    chat_server cs(io_service, ep);
    try
    {
        io_service.run();
    }
    catch (std::exception &ex)
    {
        std::cout << "[" << boost::this_thread::get_id()
                  << "] Exception: " << ex.what() << std::endl;
        return 0;
    }
}