#include "database.h"
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
