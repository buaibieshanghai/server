#include "database.h"
#include <boost/asio.hpp>

#include <iostream>
Database::Database(){
    conn = new MYSQL();
    mysql_init(conn);
    if (mysql_real_connect(conn, "47.95.145.100", "root", "336699369sysSYS@", "Student_payment_database", 0, NULL, CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
    {
        std::cout <<("数据库连接成功\n")<<std::endl;
    }
    if(mysql_query(conn, "set names \'utf8\'"))  //这个是关键
    {
        std::cout<<mysql_error(conn)<<std::endl;
    }
}
 Database::~Database(){
    delete conn;
    delete _fd;
    delete _res;
 // 释放结果集
 
    mysql_free_result(_res);
 
    // 关闭Mysql连接
 
    mysql_close(conn);

}
bool Database::setSQL(const char *ch){
std::cout << "执行的sql语句为"<<ch<<std::endl;
            if ( mysql_query(conn, ch))
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

boost::json::object Database::getData(int index){
 boost::json::object  val;
 

    switch(index){

        case 1:
        break;
        case 2:
    _res= mysql_store_result(conn);
        _row = mysql_fetch_row(_res);
       val= {
    {"code", "2"},
    {"user_name",_row[1]},
    {"user_sex",_row[2]},
    {"user_type",_row[3]},
    {"user_xueyuan",_row[4]},
    {"user_zhuanye",_row[5]},
    };// mysql返回一行个人信息

        break;
        case 3:
        break;

        case 4:

        _row = mysql_fetch_row(_res);
if(_row==NULL){

val={{"code",0}};
    return val;
}
       val= {
    {"code", 5},
    {"buy_time",_row[1]},
    {"buy_food",_row[2]},
    {"buy_price",_row[3]},
    {"buy_user_id",_row[4]},
       {"buy_image_id",_row[5]}
    };// mysql返回一行个人信息
        break;
 
        case 6:

    _res= mysql_store_result(conn);
        _row = mysql_fetch_row(_res);
if(_row==NULL){

val={{"code",0}};
    return val;
}
       val= {
    {"code",6},
{"login_result",1},
    {"user_image_id",_row[0]},
    };// mysql返回一行个人信息
break;
  case 7:

    _res= mysql_store_result(conn);
        _row = mysql_fetch_row(_res);
       val= {
    {"code",7},
    {"user_yue",_row[0]},
    };// mysql返回一行个人信息
break;
case 8:

        _row = mysql_fetch_row(_res);
if(_row==NULL){

val={{"code",0}};
    return val;
}
       val= {
    {"code", 8},
    {"buy_time",_row[1]},
    {"buy_food",_row[2]},
    {"buy_price",_row[3]},
    {"buy_user_id",_row[4]},
       {"buy_image_id",_row[5]}
    };// mysql返回一行个人信息
break;

case 9:
    _res= mysql_store_result(conn);
        _row = mysql_fetch_row(_res);
       val= {
    {"code", 9},
    
    {"user_id",_row[0]},
    {"user_name",_row[1]},
    {"user_sex",_row[2]},
    {"user_type",_row[3]},
    {"user_xueyuan",_row[4]},
       {"user_zhuanye",_row[5]}
    };// mysql返回一行个人信息
std::cout <<"cs"<<std::endl;
break;
    }
   std::cout<<"json数据准备成功"<<std::endl;
    // 返回查询结果集
// 从查询结果集中读取数据



return val;
}


