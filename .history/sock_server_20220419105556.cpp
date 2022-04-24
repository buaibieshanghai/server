#include "sock_server.h"
#include "share_sock.h"
#include <iostream>
sock_server::sock_server(boost::asio::io_service &io_service, const tcp::endpoint &endpoint) : acceptor_(io_service, endpoint), socket_(io_service)
{
    do_accept();
}
void sock_server::do_accept()
{
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec){
        if(!ec){
            // 有客户端连接 创建一个socket的智能指针 并且初始化加入聊天室
             auto session=
             std::make_shared<Share_sock>(std::move(socket_),room_,db_);
             session->start();
 std::cout <<"有新客户端连接"<<std::endl;
        }
        do_accept(); });
}