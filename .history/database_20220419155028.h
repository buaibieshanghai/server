
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
// 数据库查询结果(一行)
MYSQL_ROW _row;  
执行查询语句
bool setSQL(const char *ch);
返回查询结果
boost::json::object getData(int index);

};

#endif