#include "share_sock.h"
void Sock_room::join(std::shared_ptr<Share_sock> share_sock)
{
     share_sock_que.insert(share_sock);
}
void Sock_room::leave(std::shared_ptr<Share_sock>  share_sock)
{
   share_sock_que.erase(share_sock);
}
// 分发消息
void Sock_room::deliver(boost::json::object val)
{
    for (auto share_sock:share_sock_que)
    {
        share_sock->deliver(val);
    }
}
