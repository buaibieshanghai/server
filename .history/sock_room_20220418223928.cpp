#include "share_sock.h"
void Sock_room::join(Share_sock participant)
{
    participants_.insert(participant);
    // for(const auto& msg:recent_msgs_){
    //     // participant->deliver(msg);
    // }
}
void Sock_room::leave(Share_sock participant)
{
    participants_.erase(participant);
}
// 分发消息
void Sock_room::deliver(boost::json::object val)
{
    for (auto participant :share_sock_que)
    {
        std::cout << participants_.size() << std::endl;
        participant->deliver(val);
    }
}
