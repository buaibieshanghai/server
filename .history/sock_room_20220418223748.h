#include "share_sock.h"
class Sock_room
{
public:
    void join(Share_sock);
    void leave(Share_sock);
    void deliver(boost::json::object val);

private:
    // 用set存放基类的智能指针
    std::set<Share_sock> share_sock_que;
};