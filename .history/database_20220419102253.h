
#ifndef __Database__h
#define __Database__h

#include "database.h"
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

#include <deque>
#include <list>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <boost/system/system_error.hpp>
#include <cstdlib>
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