#include "database.h"
#include <iostream>
#include <list>
#include <boost/asio.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/value.hpp>
#include <boost/json/string.hpp>
#include <boost/format.hpp>
#include <boost/json.hpp>
#include <string>
#include <boost/json/object.hpp>
#include <list>

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/object.hpp>
#include <boost/json/value.hpp>
#include <boost/json/string.hpp>
#include <boost/format.hpp>
#include <boost/json.hpp>
#include <boost/format/free_funcs.hpp>
#include <string>
Database::Database()
{
    conn = new MYSQL();
    mysql_init(conn);
    if (mysql_real_connect(conn, "47.95.145.100", "root", "336699369sysSYS@", "Student_payment_database", 0, NULL, CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
    {
        std::cout << ("数据库连接成功\n") << std::endl;
    }
    if (mysql_query(conn, "set names \'utf8\'")) //这个是关键
    {
        std::cout << mysql_error(conn) << std::endl;
    }
}
Database::~Database()
{
    delete conn;
    delete _res;
    // 释放结果集

    mysql_free_result(_res);

    // 关闭Mysql连接

    mysql_close(conn);
}
bool Database::setSQL(const char *ch)
{
    std::cout << "执行的sql语句为" << ch << std::endl;
    if (mysql_query(conn, ch))
    {
        printf("执行sql语句失败\n");
        return false;
    }
    else
    {
        printf("执行sql语句成功\n");

        return true;
    }
}

// 返回对应业务的json的列表
// 1.解析客户端发送的json业务值
// 2.
std::list<boost::json::object> &Database::getData(boost::json::object json)
{

    int in = json.at("code").as_int64();
    switch (in)
    {
    case 1:
        // return return_yewu_1(json);
        break;
    case 2:
        return return_yewu_2(json);
        break;
    case 3:
        // return return_yewu_3(json);
        break;
    case 4:
        // return return_yewu_4(json);
        break;
    case 5:
        // return return_yewu_5(json);
        break;
    case 6:
        // return return_yewu_6(json);
        break;
    case 7:
        // return return_yewu_7(json);
        break;
    case 8:
        // return return_yewu_8(json);
        break;
    }
}

std::list<boost::json::object> &Database::return_yewu_2(boost::json::object &json)
{

    std::list<boost::json::object> list_json;
    bool result1 = setSQL(str(boost::format("insert into user_id_password values(%1%, \"%2%\",%3%,100)") % obj.at("user_zhanghao").as_string().c_str() % obj.at("user_password").as_string().c_str() % obj.at("user_image").as_string().c_str()).c_str());

    bool result2 = setSQL(str(boost::format("insert into user_baseinformation values(%1%,\"%2%\", \"%3%\", \"%4%\", \"%5%\",\"%6%\")") % obj.at("user_zhanghao").as_string().c_str() % obj.at("user_name").as_string().c_str() % obj.at("user_sex").as_string().c_str() % obj.at("user_type").as_string().c_str() % obj.at("user_xueyuan").as_string().c_str() % obj.at("user_zhuanye").as_string().c_str()).c_str());

    if (result1 && result2)
    {
        list_json.push_back({{"code", 1},
                             {"enroll_result", 1}}); // mysql返回一行个人信息
    }
    else
    {

        list_json.push_back({

            {"code", 1},
            {"enroll_result", 0}}); // mysql返回一行个人信息
    }
    return list_json;
}

// std::list<boost::json::object> &Database::return_yewu_4(boost::json::object &json)
// {
//     std::list<boost::json::object> list_json;
//     // 拉去服务器的历史消息并且全部发送给服务器
//     bool select_result = setSQL("select *from msg");

//     _res = mysql_store_result(conn);
//     int i = 0;
//     _row = mysql_fetch_row(_res);
//     while (_row != NULL)
//     {
//         i++;
//         list_json.push_back({{"code", 5},
//                              {"buy_time", _row[1]},
//                              {"buy_food", _row[2]},
//                              {"buy_price", _row[3]},
//                              {"buy_user_id", _row[4]},
//                              {"buy_image_id", _row[5]}});
//         _row = mysql_fetch_row(_res);
//     }
//     std::cout << "共查询到" << i << "条购买记录" << std::endl;
//     return list_json;
// }

// std::list<boost::json::object> &Database::return_yewu_5(boost::json::object &obj)
// {

//     //业务5 将用户的购买信息上传到服务器   物品价格 购买的人id 图片id
//     // 接受到一个客户端的信息 调用room中的函数 将结果传送给所有人 并且修改数据库中的余额
//     setSQL(str(boost::format("insert into msg values(0,\"%1%\",\"%2%\",%3%,%4%,%5%)") % obj.at("buy_time").as_string().c_str() //
//                % obj.at("buy_food").as_string().c_str()                                                                        //
//                % obj.at("buy_price").as_string().c_str()                                                                       //
//                % obj.at("buy_user_id").as_string().c_str()                                                                     //
//                % obj.at("buy_image_id").as_string().c_str())
//                .c_str()); //

//     setSQL(str(boost::format("UPDATE user_id_password SET user_yue=user_yue-%1% where user_zhanghao=%2%") % obj.at("buy_price").as_string().c_str() //
//                % obj.at("buy_user_id").as_string().c_str()                                                                                          //
//                )
//                .c_str());
//     std::list<boost::json::object> list_json;
//     return list_json;
// }

// std::list<boost::json::object> &Database::return_yewu_6(boost::json::object &obj)
// {

//     std::list<boost::json::object> list_json;
//     bool result = setSQL(str(boost::format("select user_image from user_id_password where user_zhanghao=%1% and user_password=\"%2%\"") % obj.at("m_user_id").as_string().c_str() % obj.at("m_user_password").as_string().c_str()).c_str()

//     );

//     _res = mysql_store_result(conn);
//     _row = mysql_fetch_row(_res);
//     if (_row == NULL)
//     {

//         // 没有结果 账号密码错误 返回错误信息
//         list_json.push_back({{"code", 6},
//                              {"login_result", 0}

//         });
//     }
//     else
//     {
//         list_json.push_back({
//             {"code", 6},
//             {"login_result", 1},
//             {"user_image_id", _row[0]},
//         });
//         return list_json;
//     }
// }
// std::list<boost::json::object> &Database::return_yewu_7(boost::json::object &obj)
// {

//     std::list<boost::json::object> list_json;
//     setSQL(str(boost::format("select user_yue from user_id_password where user_zhanghao= %1%") % obj.at("m_user_id").as_int64()).c_str());
//     _res = mysql_store_result(conn);
//     _row = mysql_fetch_row(_res);

//     list_json.push_back({
//         {"code", 7},
//         {"user_yue", _row[0]},
//     });
//     setSQL(str(boost::format("select * from user_baseinformation  where user_id= %1%") % obj.at("m_user_id").as_int64()).c_str());

//     _res = mysql_store_result(conn);
//     _row = mysql_fetch_row(_res);
//     list_json.pushback({{"code", 9},

//                         {"user_id", _row[0]},
//                         {"user_name", _row[1]},
//                         {"user_sex", _row[2]},
//                         {"user_type", _row[3]},
//                         {"user_xueyuan", _row[4]},
//                         {"user_zhuanye", _row[5]}}); // mysql返回一行个人信息

//     return list_json;
// }

// std::list<boost::json::object> &Database::return_yewu_7(boost::json::object &obj)
// {

//     std::list<boost::json::object> list_json;
//     setSQL(str(boost::format("select * from msg where msg_user_id= %1%") % obj.at("m_user_id").as_int64()).c_str());

//     _res = mysql_store_result(conn);
//     _row = mysql_fetch_row(_res);

//     while (_row != NULL)
//     {
//         list_json.push_back({{"code", 8},
//                              {"buy_time", _row[1]},
//                              {"buy_food", _row[2]},
//                              {"buy_price", _row[3]},
//                              {"buy_user_id", _row[4]},
//                              {"buy_image_id", _row[5]}}); // mysql返回一行个人信息
//         _row = mysql_fetch_row(_res);
//     }
//     return list_json;
// }


