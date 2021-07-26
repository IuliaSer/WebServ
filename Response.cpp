#include "main.hpp"
#include <iostream>
#include <string>

std::string current_date(){
    std::string date("Date: ");
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    date.append(asctime(timeinfo));
    return date;
}

void Response::find_root(Request &zapros) {
    _root = _hosts_and_root.find(zapros.getHost())->second;
}

void Response::autoindexOn()
{
	DIR *dir;
	struct dirent *current;
	std::string body;
	std::string relativePath = ".";

    chdir(_root.c_str());
    std::cout << "ROOT autoindex" << _root << std::endl;
	dir = opendir(relativePath.c_str());
	if (dir == NULL)
	{
		std::cout << "AUTOINDEX\n";
        perror("error");
		return ;
	}
	body = "<html>\n<head>";
	body += "<title>webserv - AutoIndexOn</title>\n"
			"<style> "
			" * { margin: 0; padding: 0; }"
			"h1 { text-align: center; font-size: 25px; margin-top: 30px;}"
			"a { text-decoration: none; color: black; font-size: 20px;}"
			"body { bac     kground: rgb(238,174,202); background: radial-gradient(circle, rgba(238,174,202,1) 0%, rgba(148,187,233,1) 100%); }"
			"ul { display: flex; flex-wrap: wrap; flex-direction: column; border-radius: 5px; text-align: center; padding-left: 0; margin-top: 30px;}"
			"li { display: block; border-bottom: 1px solid #673ab7; padding-bottom: 5px;}"
			"</style>\n</head>\n<body>\n";
	body += "<h1>Autoindex On:</h1>\n<ul>";
	while ((current = readdir(dir)) != NULL)
	{
		if (current->d_name[0] != '.')
		{
			body += "<li><a href=\"";
			body += current->d_name;
			body += "\">";

			body += current->d_name;
			body += "</a></li>";
		}
	}
	closedir(dir);
	body += "</ul></body>\n</html>\n";
	this->_answer_body = body;
}

int check_file_location(std::string const &file_path){
    struct stat fl_stat;
    if (stat(file_path.c_str(), &fl_stat) != 0)
       return (404);
    return (0);
}

void file_read(std::string const &location, std::string &answer_body){

    std::string line;
    std::ifstream in;
    if (location.find(".jpg") != std::string::npos){
        in.open(location.c_str());
    }
    else
        in.open(location.c_str()); // окрываем файл для чтения
    if (in.is_open())
    {
        in.seekg(0, in.end);
        long length = in.tellg();
        in.seekg(0, in.beg);

        char *buffer = new char [length];
        in.read(buffer, length);
        answer_body.append(buffer, in.gcount());//надо добавить проверки
        delete[] buffer;
    }
    in.close();     // закрываем файл
}

std::string Response::error_404(std::string const &key /* 127.0.0.1:8081*/){
	std::ifstream 	ifs;
	std::stringstream ss;
	std::string 	path;
	std::string		file;

	std::string error_answer;
	path.clear();
	path = _hosts_and_root.find(key)->second;
	path += _default_errors.find(key)->second.find(404)->second;
	ifs.open(path);
	if (!ifs.is_open()){
		error_answer = "HTTP/1.1 404 Not Found\r\nServer: my_webserver\r\n";
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: 139\r\n"
						"Connection: close\r\n"
						"\r\n"
						"<html>"
                    	"<head><title>404 Not Found</title></head>"
                    	"<body>"
                    	"<center><h1>404 Not Found</h1></center>"
                    	"<hr><center>My_webserver</center>"
                    	"</body>"
                    	"</html>";
	}
	else {
		std::getline(ifs, file, ifs.widen(EOF));
		ifs.close();
		ss << file.length();
		error_answer = "HTTP/1.1 404 Not Found\r\nServer: my_webserver\r\n";
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: " + ss.str() + "\r\n"
														"Connection: close\r\n"
														"\r\n"
						+ file;
	}

	std::cout << error_answer; // todo for debug
    return error_answer;
}

std::string Response::error_400(std::string const &key){
	std::ifstream 	ifs;
	std::stringstream ss;
	std::string 	path;
	std::string		file;

	std::string error_answer;
	path.clear();
	path = _hosts_and_root.find(key)->second;
	path += _default_errors.find(key)->second.find(400)->second;
	ifs.open(path);
	if (!ifs.is_open()) {
		error_answer = "HTTP/1.1 400 Bad Request\r\nServer: my_webserver\r\n";
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: 143\r\n" //todo
						"Connection: close\r\n"
						"\r\n"
						"<html>"
	                    "<head><title>400 Bad Request</title></head>"
	                    "<body>"
	                    "<center><h1>400 Bad Request</h1></center>"
	                    "<hr><center>My_webserver</center>"
	                    "</body>"
	                    "</html>";
	}
	else {
		std::getline(ifs, file, ifs.widen(EOF));
		ifs.close();
		ss << file.length();

		error_answer = "HTTP/1.1 400 Bad Request\r\nServer: my_webserver\r\n";
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: " + ss.str() + "\r\n"
														"Connection: close\r\n" //надо закрыть соединение после такого ответа
														"\r\n" + file;
	}
	std::cout << error_answer;
    return error_answer;
}

std::string Response::error_403(std::string const &key){
	std::ifstream 	ifs;
	std::stringstream ss;
	std::string 	path;
	std::string		file;

	std::string error_answer;
	path.clear();
	path = _hosts_and_root.find(key)->second;
	path += _default_errors.find(key)->second.find(403)->second;
	ifs.open(path);
	if (!ifs.is_open()){
		error_answer = "HTTP/1.1 403 Forbidden\r\nServer: my_webserver\r\n"; // todo changed 400 to 403
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: 139\r\n" //todo
						"Connection: close\r\n"
						"<html>"
	                    "<head><title>403 Forbidden</title></head>"
	                    "<body>"
	                    "<center><h1>403 Forbidden</h1></center>"
	                    "<hr><center>My_webserver</center>"
	                    "</body>"
	                    "</html>";
	}
	else {
		std::getline(ifs, file, ifs.widen(EOF));
		ifs.close();
		ss << file.length();

		error_answer = "HTTP/1.1 403 Forbidden\r\nServer: my_webserver\r\n"; // todo changed 400 to 403
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: " + ss.str() + "\r\n"
														"Connection: close\r\n" //надо закрыть соединение после такого ответа
														"\r\n" + file;
	}
    return error_answer;
}

std::string Response::error_405(std::string const &key){
	std::ifstream 	ifs;
	std::stringstream ss;
	std::string 	path;
	std::string		file;

	std::string error_answer;
	path.clear();
	path = _hosts_and_root.find(key)->second;
	path += _default_errors.find(key)->second.find(405)->second;
	ifs.open(path);
	if (!ifs.is_open()){
		error_answer ="HTTP/1.1 405 Not Allowed\r\nServer: my_webserver\r\n";
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: 143\r\n" //todo
						"Connection: close\r\n"
						"<html>"
						"<head><title>405 Not Allowed</title></head>"
						"<body>"
						"<center><h1>405 Not Allowed</h1></center>"
						"<hr><center>My_webserver</center>"
						"</body>"
						"</html>";
	}
	else {
		std::getline(ifs, file, ifs.widen(EOF));
		ifs.close();
		ss << file.length();

		error_answer ="HTTP/1.1 405 Forbidden\r\nServer: my_webserver\r\n";
		error_answer += current_date();
		error_answer += "Content-Type: text/html\r\n"
						"Content-Length: " + ss.str() + "\r\n"
														"Connection: close\r\n" //надо закрыть соединение после такого ответа
														"\r\n" + file;
	}
	return error_answer;
}

void Response::error_413()
{
    std::string error_answer("HTTP/1.1 413 Payload Too Large\r\nServer: my_webserver\n");
    error_answer += current_date();
    error_answer += "Content-Type: text/html\n"
                    "Content-Length: 154\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "<html>"
                    "<head><title>413 Payload Too Large</title></head>"
                    "<body>"
                    "<center><h1>413 Payload Too Large</h1></center>"
                    "<hr><center>My_webserver</center>"
                    "</body>"
                    "</html>";
    _answer = error_answer;
}

std::string content_type(std::string const &file_path) {
    int extentionOfScript = 0;
    if (file_path.find(".html") != std::string::npos)
        return ("text/html");
    else if (file_path.find(".jpg") != std::string::npos)
        return ("image/jpeg");
    else if (file_path.find(".ico") != std::string::npos)
        return ("image/jpeg");
    else if (file_path.find(".css") != std::string::npos)
        return ("text/css");
    else if (file_path.rfind(".py") != std::string::npos)
		return("python");
    else if (file_path.rfind(".cgi") != std::string::npos || file_path.rfind(".exe") != std::string::npos)
    {
        extentionOfScript = 1;
        return("cgi");
    }
    else
        return("");
}

void    Response::fill_hosts_and_root(std::vector<Server>& servers)
{
    std::vector<Server>::iterator it = servers.begin();
    while (it != servers.end()){
        _hosts_and_root.insert(std::make_pair(it->getHost() + ":" + it->getPort(), it->getRoot()));
        _default_errors.insert(std::make_pair(it->getHost() + ":" + it->getPort(), it->_default_error_page));
        it++;
    }
}

std::string	Response::getStatus(int code)
{
	if (_errors.find(code) != _errors.end())
		return _errors[code];
	return ("Unknown Code");
}

void Response::ErrorsValue()
{
	_errors[100] = "Continue";
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[400] = "Bad Request";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";
}

void Response::make_headers(Request &zapros)
{
        if(_code == 200 || _code == 201)
        {
            _answer = "HTTP/1.1 " + std::to_string(_code) + " " + getStatus(_code) + "\r\n";
            int length = static_cast<int>(_answer_body.length());
            char buffer[1000];
            sprintf(buffer, "%d", length);
            std::string length_string(buffer);
            std::cout << "lenght - > " << length_string << std::endl;
            _answer += "Server: my_webserver\r\n";
            _answer += current_date();
            _answer += "Content-Type: ";
            _answer += _content_type;
            _answer += "\r\n";
            _answer += "Content-Length: ";
            _answer += length_string;
            _answer += "\r\n";
            if (_code == 201)
            {
                _answer += "Location: ";
                _answer += zapros.getLocation();
                _answer += "\r\n";
            }
            _answer += "\r\n";
            _answer += _answer_body;
        }
}

void Response::setValues(Request &zapros)
{
    _code = 200;
    _AUTOINDEX = 0;
    _file_path = _root + zapros.getResourseName();
    _content_type = content_type(_file_path);
}

void Response::resetValues(Request &zapros)
{
    _content_type = zapros.getContentType();
    _answer_body = zapros.getAnswerBody();
}

void Response::check_location(Request zapros, std::vector<Server>& servers)
{
    Server serv;
    std::string port;
    int size = zapros.getHost().size();
    std::string dir_path;
    int j = -1;
    int f = zapros.getHost().find(":", 0);
    port = zapros.getHost().substr(f + 1, size - (f + 1));

    int i = zapros.getResourseName().rfind(".", zapros.getResourseName().size());
    if(i > 0)
        j = zapros.getResourseName().rfind("/", i); // '/' position
    int pos = zapros.getResourseName().size() - (zapros.getResourseName().size() - j - 1);
    if (j >= 0)
        dir_path = zapros.getResourseName().substr(0, pos);
    else 
        dir_path = zapros.getResourseName();
    for (int i = 0; i < servers.size(); i++)
    { 
        if (servers[i]._port == port)
        {
            for (int j = 0; j < servers[i]._locations.size(); j++)
            {
                std::cout << "PATH: " << servers[i]._locations[j]._path << std::endl;
                std::cout << "Resourse_name: " << zapros.getResourseName() << std::endl;
                if(servers[i]._locations[j]._path == dir_path) //mi na nuzhnom location
                {
                    _root = servers[i]._locations[j]._root;
                    if (zapros.getResourseName() == dir_path)
                        _file_path = _file_path + "/" + servers[i]._locations[j]._index[0];
                    std::cout << "FILE_PATH: " << _file_path << std::endl;
                    int a = 0;
                    for (; a < servers[i]._locations[j]._allowed_methods.size(); a++)
                    {
                        if(zapros.getMethod() == servers[i]._locations[j]._allowed_methods[a])
                            break;
                    }
                    if (a == servers[i]._locations[j]._allowed_methods.size()) // doshla do konca vectora
                    {
                        _answer = error_405(zapros.getHost()); // poka net 405
                        _code = 405;
                    }
                    else if(servers[i]._locations[j]._redirects.count(301))
                    {
                        _file_path = servers[i]._locations[j]._redirects[301] + "/" + servers[i]._locations[j]._index[0];
                    }
                    else if(servers[i]._locations[j]._autoindex)
                    {
                        autoindexOn();
                        _AUTOINDEX = 1;
                    }
                    return;
                }
            }
        }
    }
    return;
}

void Response::make_get_response(Request zapros, std::vector<Server>& servers)
{
    if (check_file_location(_file_path) == 404)
    {
        _answer = error_404(zapros.getHost());
        _code = 404;
        return;
    }
    check_location(zapros, servers);
    if(_code == 200 && _AUTOINDEX == 0)
        file_read(_file_path, _answer_body);
    make_headers(zapros);
}

void Response::make_delete_response(Request &zapros, std::vector<Server>& servers)
{
    if (check_file_location(_file_path) == 404)
    {
        _answer = error_404(zapros.getHost());
        _code = 404;
        return;
    }
    check_location(zapros, servers);
    if(_code == 200)
    {
        if (!remove(_file_path.c_str()))
                _answer = error_403(zapros.getHost());
        make_headers(zapros);
    }
}

void Response::make_post_response(Request &zapros)
{
    Cgi c;
    if (check_file_location(_file_path) == 404)
    {
        _answer = error_404(zapros.getHost());
        _code = 404;
        return;
    }
    c.execute_cgi(zapros, _file_path);
    resetValues(zapros);
    _code = std::atoi(zapros.getStatus().c_str());
    make_headers(zapros);
}

void Response::choose_method(Request &zapros, std::vector<Server>& servers)
{
    ErrorsValue();
    find_root(zapros);
    setValues(zapros);
    if (zapros.getMethod() == "GET")
        make_get_response(zapros, servers);
    else if (zapros.getMethod() == "DELETE")
        make_delete_response(zapros, servers);
    else if (zapros.getMethod() == "POST")
        make_post_response(zapros);
    else 
        _answer = error_400(zapros.getCurrentServer()._host + ":" + zapros.getCurrentServer().getPort()); // заменить все answer 1 прочитать из файла в answer
}