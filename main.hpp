#ifndef MAIN_HPP
#define MAIN_HPP
#include <iostream>
#include <netdb.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio> //perror
#include <set>
#include <ctime>
#include <algorithm>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h> //stat
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fstream>
#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Cgi.hpp"
#include <vector>
#define ROOT "/Users/iserzhan/WebServ/site/"

#define PORT "8000"

#define CGI_BUFSIZE 65536

#endif