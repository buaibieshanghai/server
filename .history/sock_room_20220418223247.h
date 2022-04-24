
class Sock_room
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