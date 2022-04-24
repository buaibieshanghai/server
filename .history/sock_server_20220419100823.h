
#ifndef __SOCKSERVER__h
#define __SOCKSERVER__h
#include <boost/asio.hpp>
#include "sock_room.h"
#include "share_sock.h"
#include "database.h"

using boost::asio::ip::tcp;
class sock_server
{
public:
     sock_server(boost::asio::io_service &, const tcp::endpoint &);
private:
    void do_accept();
private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
 Sock_room room_;
    Database db_;
};

#endif
