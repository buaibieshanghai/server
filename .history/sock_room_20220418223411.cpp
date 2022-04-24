#include "share_sock.h"
    void  Sock_room::join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        // for(const auto& msg:recent_msgs_){
        //     // participant->deliver(msg);
        // }
    }
    void Sock_room::leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }
    // 分发消息
    void Sock_room::deliver(boost::json::object val)
    {
        for (auto participant : participants_)
        {
            std::cout << participants_.size() << std::endl;
            participant->deliver(val);
        }
    }
