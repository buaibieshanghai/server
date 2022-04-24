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
using namespace chat_participant_ptr = std::shared_ptr<Share_sock>;
class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        // for(const auto& msg:recent_msgs_){
        //     // participant->deliver(msg);
        // }
    }
    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }
    // 分发消息
    void deliver(boost::json::object val)
    {
        for (auto participant : participants_)
        {
            std::cout << participants_.size() << std::endl;
            participant->deliver(val);
        }
    }

private:
    // 用set存放基类的智能指针
    std::set<chat_participant_ptr> participants_;
};
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