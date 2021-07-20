#include "main.hpp"

void *get_client_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    int listen_sock;
    ssize_t bytes_read;
    char buf[65000];
    int yes = 1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // указывает, что будет IPv4
    hints.ai_socktype = SOCK_STREAM; //указывает, что сокет будет потоковый (TCP)
    hints.ai_flags = AI_PASSIVE; //указывает, что структурам сокета нужно назначить адрес локального хоста
    if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        std::cout << gai_strerror(status) << std::endl;
        exit(1);
    }
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        listen_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(listen_sock == -1)
            continue;
        if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(int)) == -1)
        {
            fprintf(stderr, "error setsockopt\n");
            close(listen_sock);
            freeaddrinfo(servinfo); // all done with this structure
            return -2;
        }
        if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
        {
            fprintf(stderr, "error 123 setsockopt\n");
            close(listen_sock);
            freeaddrinfo(servinfo); // all done with this structure
            return -2;
        }
        if(bind(listen_sock, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("bind");
            close(listen_sock);
            continue;
        }
        std::cout << "SOCKET CREATED SUCCESFULLY" << std::endl;
        break;
    }
    freeaddrinfo(servinfo);
    listen(listen_sock, 5);
    std::cout << "listen fd " << listen_sock << std::endl;
    std::set<int> clients;
    clients.clear();
    fd_set master;
    fd_set readset;
    FD_ZERO(&master);
    FD_ZERO(&readset);
    FD_SET(listen_sock, &master);
    int fdmax = listen_sock;
    while (1) {
        readset = master;

        timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        if(select(fdmax + 1, &readset, NULL, NULL, &timeout) == -1)
        {
            perror("select");
            exit(3);
        }

        // std::cout << "still running" << std::endl;
        // std::cout << "Fdmax ->" << fdmax << std::endl;
        struct sockaddr_storage client_addr;
        socklen_t s_size = sizeof(client_addr);
        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &readset)) {
                if (i == listen_sock) {
                    int sock = accept(listen_sock, (struct sockaddr *) &client_addr, &s_size);
                    if (sock < 0) {
                        perror("accept");
                        exit(3);
                    }
                    FD_SET(sock, &master);
                    if (sock > fdmax)
                        fdmax = sock;
                    char ip[INET6_ADDRSTRLEN];
                    inet_ntop(client_addr.ss_family, get_client_addr((struct sockaddr *) &client_addr), ip, sizeof ip);
                    std::cout << "server: got connection from\n" << ip << std::endl;
                    fcntl(sock, F_SETFL, O_NONBLOCK);
                }
                else {
                    bytes_read = recv(i, buf, sizeof(buf), 0);
                    if (bytes_read <= 0) {
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else {
                        std::cout << "|....Client request : " << buf << std::endl << std::endl;
                        Request zapros;
                        Response resp;
                        zapros.clean_request();
                        zapros.parse_request(buf);
                        resp.choose_method(zapros);
                        ssize_t res = send(i, resp.getAnswer().c_str(), resp.getAnswer().length(), 0);
                        std::ofstream log("log.txt", std::ios_base::trunc);
                        log << "Возвращаемое значение send = " << res << std::endl;
                        log << resp.getAnswer() << std::endl;
                        log.close();
                        if (resp.getAnswer().find("Connection: close\n")){
                            close(i);
                            FD_CLR(i, &master);
                        }
                    }
                }
            }
        }
    }
    return 0;

}