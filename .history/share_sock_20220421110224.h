
#ifndef __SHARESOCK__h
#define __SHARESOCK__h
#include <boost/asio.hpp>
#include <boost/json/object.hpp>

#include "data_json.h"

#include <memory>
#include <list>
class Database;
class Sock_room;
using boost::asio::ip::tcp;
class Share_sock : public std::enable_shared_from_this<Share_sock>
{
public:

    Share_sock(tcp::socket socket, Sock_room & room, Database &db);
    void start();
    void deliver(boost::json::object var);
    // // 对客户端发送的业务处理
    void action(size_t len);
    void do_read_baotou();
    void do_read_baoti(size_t len);
    // //像客户端发送数据
    void do_write();
private:
    tcp::socket socket_;
    Sock_room &room_;
    // 发送json的队列
    bool index_read=false;
    std::list<Data_json*> char_list; 
    char data[512];
    Database &db_;
};

#endif
