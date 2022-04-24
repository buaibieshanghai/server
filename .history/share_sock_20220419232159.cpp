#include "share_sock.h"
#include "sock_room.h"
#include "database.h"
#include <iostream>

#include <boost/asio.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/value.hpp>
#include <boost/json/string.hpp>
#include <boost/format.hpp>
#include <boost/json.hpp>

Share_sock::Share_sock(tcp::socket socket, Sock_room &room, Database &db) : socket_(std::move(socket)), room_(room), db_(db)
{
    std::cout << &room_ << std::endl;
    std::cout << &db_ << std::endl;
}

void Share_sock::start()
{
    room_.join(shared_from_this());
    do_read_baotou();
}

void Share_sock::deliver(boost::json::object var)
{

    std::cout << var << std::endl;
    boost::json::serializer ser;
    ser.reset(&var);
}
void Share_sock::do_read_baotou()
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

void Share_sock::do_read_baoti(size_t len)
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

void Share_sock::do_write()
{
    auto self(shared_from_this());
    // 获取队列头部的json数据
    // 必须用引用 不然会发生多次delete
    auto &msg = char_list.front();
    std::cout << "待发送" << std::endl;
std::cout<<msg.data<<std::endl;
std::cout<<msg.len<<std::endl;
    boost::asio::async_write(

        socket_, boost::asio::buffer(msg.data, msg.len),
        [this, self](boost::system::error_code ec, std::size_t len)
        {
            // read事件触发回调函数
    std::cout << "送" << std::endl;
            if (ec)
            {
                if (!char_list.empty())
                    char_list.clear();
                std::cout << "客户端断开" << std::endl;
                room_.leave(shared_from_this());
                std::cout << "已释放客户端资源" << std::endl;
            }
            else
            // 移除头部的json消息
            {

                char_list.pop_front();
                std::cout << "当前消息队列个数为"<<char_list.size() << std::endl;
                std::cout << "已发送" << std::endl;
                if (!char_list.empty())
                {
                    do_write();
                }
            }
        });
        std::cout<<"kk"<<std::endl;
}
// 把客户端发来的 byte数组 转换成 byte数组发送出去
void Share_sock::action(size_t len)
{
    // 异步读取客户端发送的业务
    boost::json::parser p;
    // 必须保证是完整的json有效语句
    p.write(data, len);
    boost::json::object obj = p.release().as_object();

    std::cout << "已收到" << obj << std::endl;
    std::list<boost::json::object> list_json = db_.getData(obj);
    std::cout << list_json.size() << std::endl;
    // 将josn队列中的数据转化成二进制加入到发送队列
    boost::json::serializer ser;
int i=0;
    for (auto it = list_json.begin(); it != list_json.end(); it++)
    {
        i++;
        ser.reset(&(*it));
        // // // 最多发送512的数据
        // while (!ser.done())
        // {
        //     // 具有推断的
        char baoti[510];
        int index = ser.read(baoti).size();
            std::cout<<"wuwu"<<std::endl;
        char *ch = new char[index+4];
            std::cout<<"wuwu"<<std::endl;
        sprintf(ch, "%4d%.*s", index, index, baoti);
            std::cout<<"wuwu"<<std::endl;
        // 必须加判定条件 不然可能会连续发送
 bool b= char_list.empty();
            std::cout<<"wuwu"<<std::endl;
        char_list.emplace_back(ch,index+4);
            std::cout<<"wuwu"<<std::endl;
        if (b)
        {
            std::cout<<"wuwu<<"<<std::endl;

        do_write();
        }
        // }
    }
    
    std::cout<<i<<std::endl;
    std::cout<<"over"<<std::endl;
}

