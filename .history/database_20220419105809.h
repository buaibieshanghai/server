
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
MYSQL_FIELD *_fd;
MYSQL_RES *_res;
              MYSQL_ROW _row;  
bool setSQL(const char *ch);
boost::json::object getData(int index);

};

#endif