#include "share_sock.h"
void sock_room::join(share_sock share_sock)
{
    share_sock.insert(share_sock);
}
void sock_room::leave(share_sock share_sock)
{
    share_sock.erase(share_sock);
}
// 分发消息
void sock_room::deliver(boost::json::object val)
{
    for (auto share_sock:share_sock_que)
    {
        share_sock->deliver(val);
    }
}
