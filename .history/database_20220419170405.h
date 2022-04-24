
#ifndef __Database__h
#define __Database__h

#include <mysql.h>
#include <boost/json/object.hpp>

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
boost::json::object getData(int index);
std::list<boost::json::object> Database::getData(boost::json::object json);
};

#endif