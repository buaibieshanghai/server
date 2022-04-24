
#ifndef __Database__h
#define __Database__h
#include <mysql.h>
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

#endif
class Database{

public:
Database();
~Database();
public:

MYSQL *conn;
MYSQL_FIELD *_fd;
MYSQL_RES *_res;
              MYSQL_ROW _row;  
bool setSQL(const char *ch);

boost::json::object getData(int index){
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



};
