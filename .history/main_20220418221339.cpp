#include "database.h"
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
class chat_participant
{
public:
    virtual void deliver(boost::json::object val) = 0;
};
// 抽象基类的智能指
using chat_participant_ptr = std::shared_ptr<chat_participant>;
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
class chat_session : public chat_participant, public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room &room, Database &db) : socket_(std::move(socket)), room_(room), db_(db)
    {
        std::cout << &room_ << std::endl;
        std::cout << &db_ << std::endl;
    }

    void start()
    {
        boost::asio::write(socket_, boost::asio::buffer("heel", 4));
        room_.join(shared_from_this());
        do_read_baotou();
    }

public:
    void deliver(boost::json::object var)
    {
        
        std::cout << var << std::endl;
        boost::json::serializer ser;
        ser.reset(&var);
        while (!ser.done())
        {
            //     // 具有推断的
            char baoti[510];
            int index = ser.read(baoti).size();
            sprintf(data, "%4d%.*s", index, 510, baoti);
            do_write(index + 4);
        }

    }

private:
    // 对客户端发送的业务处理
    void action(size_t len)
    {
        // 异步读取客户端发送的业务
        std::cout << data << std::endl;
        boost::json::parser p;
        // 必须保证是完整的json有效语句
        p.write(data, len);
        boost::json::object obj = p.release().as_object();
        // 获取客户端的业务编码
        int in = obj.at("code").as_int64();
        boost::json::object var;
        boost::json::serializer ser;
        switch (in)
        {
            // 插入两个表
        case 1:
        {

            bool result1 = db_.setSQL(str(boost::format("insert into user_id_password values(%1%, \"%2%\",%3%,100)") % obj.at("user_zhanghao").as_string().c_str() % obj.at("user_password").as_string().c_str() % obj.at("user_image").as_string().c_str()).c_str());

            bool result2 = db_.setSQL(str(boost::format("insert into user_baseinformation values(%1%,\"%2%\", \"%3%\", \"%4%\", \"%5%\",\"%6%\")") % obj.at("user_zhanghao").as_string().c_str() % obj.at("user_name").as_string().c_str() % obj.at("user_sex").as_string().c_str() % obj.at("user_type").as_string().c_str() % obj.at("user_xueyuan").as_string().c_str() % obj.at("user_zhuanye").as_string().c_str()).c_str());
            boost::json::object val;
            if (result1 && result2)
            {
                val = {
                    {"code", 1},
                    {"enroll_result", 1}}; // mysql返回一行个人信息
            }
            else
            {

                val = {
                    {"code", 1},
                    {"enroll_result", 0}}; // mysql返回一行个人信息
            }
            ser.reset(&val);
            // // // 最多发送512的数据
            while (!ser.done())
            {
                //     // 具有推断的
                char baoti[510];
                int index = ser.read(baoti).size();
                sprintf(data, "%4d%.*s", index, 510, baoti);
                std::cout << data << std::endl;
                do_write(index + 4);
            }
        }
        break;
        case 2:
            // 业务 查询个人信息
            // 显示个人信息
            db_.setSQL("select * from user_baseinformation");
            //     // 查询结果集
            // 传入业务编号 返回要发送给客户端的json
            //  var = db_.getData(2);

            // ser.reset(&var);
            // // // 最多发送512的数据
            // char temp_buff[512];
            // while(!ser.done()){
            // //     // 具有推断的
            //     write(*sock,buffer(temp_buff,strlen(temp_buff)));
            //     std::cout << temp_buff<<std::endl;
            // }

            break;
        // 存储聊天内容
        case 4:
            // 拉去服务器的历史消息并且全部发送给服务器
            db_.setSQL("select *from msg");

            db_._res = mysql_store_result(db_.conn);

            while (((var = db_.getData(4))).at("code").as_int64() != 0)
            {

                ser.reset(&var);
                // // // 最多发送512的数据
                while (!ser.done())
                {
                    //     // 具有推断的
                    char baoti[510];
                    int index = ser.read(baoti).size();
                    sprintf(data, "%4d%.*s", index, 510, baoti);
                    do_write(index + 4);
                }
            }
            break;
        case 5:
            //业务5 将用户的购买信息上传到服务器   物品价格 购买的人id 图片id
            // 接受到一个客户端的信息 调用room中的函数 将结果传送给所有人 并且修改数据库中的余额
            db_.setSQL(str(boost::format("insert into msg values(0,\"%1%\",\"%2%\",%3%,%4%,%5%)") % obj.at("buy_time").as_string().c_str() //
                           % obj.at("buy_food").as_string().c_str()                                                                        //
                           % obj.at("buy_price").as_string().c_str()                                                                       //
                           % obj.at("buy_user_id").as_string().c_str()                                                                     //
                           % obj.at("buy_image_id").as_string().c_str())
                           .c_str()); //

            db_.setSQL(str(boost::format("UPDATE user_id_password SET user_yue=user_yue-%1% where user_zhanghao=%2%") % obj.at("buy_price").as_string().c_str() //
                           % obj.at("buy_user_id").as_string().c_str()                                                                                          //
                           )
                           .c_str());
            room_.deliver(obj);
            break;
        case 6:
        {
            bool result = db_.setSQL(str(boost::format("select user_image from user_id_password where user_zhanghao=%1% and user_password=\"%2%\"") % obj.at("m_user_id").as_string().c_str() % obj.at("m_user_password").as_string().c_str()).c_str()

            );
            // 如果执行成功 而且存在结果
            if (result && ((var = db_.getData(6))).at("code").as_int64() != 0)
            {
            }
            else
            {

                var = {
                    {"code", 6},
                    {"login_result", 0}

                }; // mysql返回一行个人信息
            }

            ser.reset(&var);
            char baoti[510];
            int index = ser.read(baoti).size();
            sprintf(data, "%4d%.*s", index, 510, baoti);
            do_write(index + 4);
            break;
        }
            // 验证客户端的账号密码信息 并且返回结果和对应账号的头像编号

        case 7:
        {

            db_.setSQL(str(boost::format("select user_yue from user_id_password where user_zhanghao= %1%") % obj.at("m_user_id").as_int64()).c_str());
            var = db_.getData(7);

            ser.reset(&var);
            char baoti[510];
            int index = ser.read(baoti).size();
            sprintf(data, "%4d%.*s", index, 510, baoti);
            do_write(index + 4);
        }
            {
                db_.setSQL(str(boost::format("select * from user_baseinformation  where user_id= %1%") % obj.at("m_user_id").as_int64()).c_str());
                var = db_.getData(9);

                ser.reset(&var);
                char baoti[510];
                int index = ser.read(baoti).size();
                sprintf(data, "%4d%.*s", index, 510, baoti);
                do_write(index + 4);
                break;
            }
        case 8:
        {

            db_.setSQL(str(boost::format("select * from msg where msg_user_id= %1%") % obj.at("m_user_id").as_int64()).c_str());

            db_._res = mysql_store_result(db_.conn);

            while (((var = db_.getData(8))).at("code").as_int64() != 0)
            {

                ser.reset(&var);
                // // // 最多发送512的数据
                while (!ser.done())
                {
                    //     // 具有推断的
                    char baoti[510];
                    int index = ser.read(baoti).size();
                    sprintf(data, "%4d%.*s", index, 510, baoti);
                    do_write(index + 4);
                }
            }
        }
        break;
        }
        // 验证客户端的账号密码信息 并且返回结果和对应账号的头像编号
    }

    void
    do_read_baotou()
    {
        //读取包头
        auto self(shared_from_this());
        boost::asio::async_read(
            socket_, boost::asio::buffer(data, 4),
            [this, self](boost::system::error_code ec, std::size_t len)
            {
                std::cout << data << std::endl;
                if (ec)
                {
                    std::cout << "客户端断开" << std::endl;
                    room_.leave(shared_from_this());
                    std::cout << "已释放客户端资源" << std::endl;
                }
                else
                {

                    do_read_baoti(atoi(data));
                } // read事件触发回调函数
            });
    }
    // 读取胞体
    void do_read_baoti(size_t len)
    {
        auto self(shared_from_this());
        boost::asio::async_read(
            socket_, boost::asio::buffer(data, len),
            [this, self](boost::system::error_code ec, std::size_t len)
            {
                // read事件触发回调函数
                if (ec)
                {
                    std::cout << "客户端断开" << std::endl;
                    room_.leave(shared_from_this());
                    std::cout << "已释放客户端资源" << std::endl;
                }
                else
                {
                    // 经过测试 boost的异步read和write事件  会在回调函数结束之后才会触发
                    action(len);
                    do_read_baotou();
                }
            });
    }
    //像客户端发送数据
    void do_write(size_t len)
    {
        auto self(shared_from_this());
        boost::asio::async_write(

            socket_, boost::asio::buffer(data, len),
            [this, self](boost::system::error_code ec, std::size_t len)
            {
                // read事件触发回调函数
                if (ec)
                {
                    std::cout << "客户端断开" << std::endl;
                    room_.leave(shared_from_this());
                    std::cout << "已释放客户端资源" << std::endl;
                }
            });
    }

private:
    tcp::socket socket_;
    chat_room &room_;
    char data[512];

    Database &db_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_service &io_service, const tcp::endpoint &endpoint) : acceptor_(io_service, endpoint), socket_(io_service)
    {
        do_accept();

        std::cout << &db_ << std::endl;
        std::cout << &room_ << std::endl;
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec)
                               {
        if(!ec){
            // 有客户端连接 创建一个socket的智能指针 并且初始化加入聊天室

            auto session=
            std::make_shared<chat_session>(std::move(socket_),room_,db_);
std::cout <<session<<std::endl;
            session->start();

std::cout <<"有新客户端连接"<<std::endl;
        }
        do_accept(); });
    }

private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    chat_room room_;
    Database db_;
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