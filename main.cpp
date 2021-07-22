#include "main.hpp"

int main() {
    char buf[65000];
    std::vector<std::string> ports;

    memset(&buf, 0, sizeof(buf));
    ports.push_back("80");
    ports.push_back("8000");
    Sockets sockets(ports);
    fd_set master;
    fd_set readset;
    fd_set writeset;
    std::map<int, Response> responses;
    FD_ZERO(&master);
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    int fdmax = sockets.listen_all(master);
    while (1) {
        readset = master;
        writeset = master;
        timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        if(select(fdmax + 1, &readset, &writeset, NULL, &timeout) == -1)
        {
            perror("select");
            exit(3);
        }
        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &readset)) {
                if (sockets.accept_connection(i, master, fdmax) == 1)
                    continue;
                else {
                    ssize_t bytes_read = 0;
                    bytes_read = recv(i, buf, sizeof(buf), 0);
                    std::cout << "bytes_read -> " << bytes_read << std::endl;
                    if (bytes_read <= 0) {
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else {
                        std::cout << "|....Client request : " << buf << std::endl << std::endl;
                        Request zapros;
                        Response resp;
                        zapros.clean_request();
                        if(!zapros.parse_request(buf))
                        {
                            resp.fill_hosts_and_root();
                            resp.choose_method(zapros);
                        }
                        responses.insert(std::make_pair(i, resp));
                    }
                }
            }
            if (FD_ISSET(i, &writeset)){
                std::map<int, Response>::iterator it = responses.find(i);
                if (it != responses.end())
                {
                    ssize_t res = send(i, it->second.getAnswer().c_str(), it->second.getAnswer().length(), 0);
                    /* Logging */
                    std::ofstream log("log.txt", std::ios_base::trunc);
                    log << "Возвращаемое значение send = " << res << std::endl;
                    log << it->second.getAnswer() << std::endl;
                    log.close();
                    /* End of Logging */
                    if (it->second.getAnswer().find("Connection: close\n"))
                    {
                        close(i);
                        FD_CLR(i, &master);
                    }
                    responses.erase(it);
                }
                
            }
        }
    }
    return 0;
}