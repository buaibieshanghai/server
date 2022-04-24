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

Share_sock::Share_sock(tcp::socket socket, Sock_room&room, Database &db) : socket_(std::move(socket)), room_(room), db_(db)
{
    std::cout << &room_ << std::endl;
    std::cout << &db_ << std::endl;
}

void Share_sock::start()
{
    boost::asio::write(socket_, boost::asio::buffer("heel", 4));
    room_.join(shared_from_this());
    do_read_baotou();
}

void Share_sock::deliver(boost::json::object var)
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

    void Share_sock::do_write(size_t len)
    {
        auto self(shared_from_this());
        boost::asio::async1_write(

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
void Share_sock::action(size_t len)
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
