
#ifndef __Database__h
#define __Database__h

#include <mysql.h>
#include <boost/json/object.hpp>
#include <list>
//负责连接mysql 并且把查询结果作为json格式的字符串返回
class Database{

public:
Database();
~Database();
public:

MYSQL *conn;

// 数据库查询结果
MYSQL_RES *_res;
// 数据库查询结果(一列)
MYSQL_ROW _row;  
bool setSQL(const char *ch);
std::list<boost::json::object> getData(boost::json::object json);
// private:

std::list<boost::json::object> return_yewu_1(boost::json::object);
std::list<boost::json::object> return_yewu_2(boost::json::object&);
std::list<boost::json::object> return_yewu_3
(boost::json::object&);
std::list<boost::json::object> return_yewu_4( boost::json::object &);
std::list<boost::json::object> return_yewu_5( boost::json::object&);
std::list<boost::json::object> return_yewu_6( boost::json::object&);
std::list<boost::json::object> return_yewu_7( boost::json::object&);
std::list<boost::json::object> return_yewu_8( boost::json::object&);

};
#endif