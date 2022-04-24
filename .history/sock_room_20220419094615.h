#ifndef __SOCKROOM__h
#define __SOCKROOM__h

#include "share_sock.h"

#include "database.h"
class Sock_room
// 客户端房间
{
public:

    void join(std::shared_ptr<Share_sock>);
    // 像room中加入新的 客户端
    void leave(std::shared_ptr<Share_sock>);
    void deliver(boost::json::object val);
    // 像所有客户端分发json
private:
    // 用set存放基类的智能指针
    std::set<std::shared_ptr<Share_sock>> share_sock_que;
};
#endif
