#ifndef WEBSERV_SOCKETS_HPP
#define WEBSERV_SOCKETS_HPP
#include "main.hpp"

class Sockets
{

    public:
        Sockets(std::vector<std::string> &ports);
        int listen_all(fd_set &master);
        std::set<int> listen_sockets;
        int accept_connection(int i, fd_set &master, int &fdmax);
    private:
        struct addrinfo _hints;
        void    fill_hints();
        void    close_all();
        void    listen_socket_setup(const char *port);
        void    reuse_address(int listen_sock, struct addrinfo *servinfo);
        void    reuse_port(int listen_sock, struct addrinfo *servinfo);
};

#endif //WEBSERV_SOCKETS_HPP
