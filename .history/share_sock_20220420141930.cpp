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

    boost::json::serializer ser;

    bool b = char_list.empty();
        ser.reset(&var);
        char baoti[510];
        int index = ser.read(baoti).size();
        // std::cout<<"sidiaole"<<std::endl;

        //!!!!!! 必须格外new一个字节的内存存放\0否则内存泄漏
        char_list.emplace_back(new Data_json(new char[index + 4 + 1], index + 4));
        sprintf(char_list.back()->data, "%4d%.*s", index, index, baoti);
        std::cout<<"是这里吗1"<<std::endl;
    if (b >0)
    {

        do_write();

        std::cout<<"是这里吗2"<<std::endl;
    }
}
void Share_sock::do_read_baotou()
{
    //读取包头
std::cout<<"a安卓"<<atoi(data)<<std::endl;
    auto self(shared_from_this());
    boost::asio::async_read(
        socket_, boost::asio::buffer(data, 4),
        [this, self](boost::system::error_code ec, std::size_t len)
        {
            if (ec)
            {
                std::cout << "客户端断开" << std::endl;
                room_.leave(shared_from_this());
                std::cout << "已释放客户端资源" << std::endl;
            }
            else
            {
std::cout<<"包头"<<atoi(data)<<std::endl;
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
    bool b = char_list.empty();
    if (b )
    {

        std::cout<<"是这里吗3"<<std::endl;
                do_read_baotou();
    }
            }
        });
}

void Share_sock::do_write()
{
    auto self(shared_from_this());
    // 获取队列头部的json数据
    // 必须用引用 不然会发生多次delete
    auto &msg = char_list.front();
    boost::asio::async_write(

        socket_, boost::asio::buffer(msg->data, msg->len),
        [this, self](boost::system::error_code ec, std::size_t len)
        {
            // read事件触发回调函数
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


                auto ls = char_list.front();
                char_list.pop_front();

                std::cout << "发送完毕的当前消息队列个数为" << char_list.size() << std::endl;
                delete ls;
                ls = NULL;
                if (!char_list.empty())
                {
                    do_write();
                }

else    {
if(read){

}
        std::cout<<"是这里吗4"<<std::endl;
                do_read_baotou();
    }
            }
        });
}
// 把客户端发来的 byte数组 转换成 byte数组发送出去
void Share_sock::action(size_t len)
{
        std::cout<<"是这里吗6"<<std::endl;
    // 异步读取客户端发送的业务
    boost::json::parser p;
    // 必须保证是完整的json有效语句
        std::cout<<len<<std::endl;
        std::cout<<data<<std::endl;
    p.write(data, len);
    boost::json::object obj = p.release().as_object();

    std::list<boost::json::object> list_json = db_.getData(obj);
    // 将josn队列中的数据转化成二进制加入到发送队列
        std::cout<<"是这里吗8"<<std::endl;
    boost::json::serializer ser;
    int i = 0;

    bool b = char_list.empty();
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
        // std::cout<<"sidiaole"<<std::endl;

        //!!!!!! 必须格外new一个字节的内存存放\0否则内存泄漏
        char_list.emplace_back(new Data_json(new char[index + 4 + 1], index + 4));
        // !!!bug

        // std::cout<<"wuwu1"<<std::endl;
        sprintf(char_list.back()->data, "%4d%.*s", index, index, baoti);
        // std::cout<<char_list.back()->data<<std::endl;
        // 必须加判定条件 不然可能会连续发送
        // }
    }

                std::cout << "读到的消息队列个数为" << char_list.size() << std::endl;
    // 必须保证加入了消息 才调用write
    
    if (b &&(i)>0)
    {

        std::cout<<"是这里吗5"<<std::endl;
        do_write();
    }

    if(obj.at("code").as_int64()==5){
room_.deliver(obj);
    // 转发业务 1.上传数据库 没有返回值 而是直接

    }
}
