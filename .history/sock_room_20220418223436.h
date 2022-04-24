#include "share_sock.h"
class Sock_room
{
public:
    void join(chat_participant_ptr participant);
    void leave(chat_participant_ptr participant);
    void deliver(boost::json::object val);

private:
    // 用set存放基类的智能指针
    std::set<chat_participant_ptr> participants_;
};