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

void Response::find_root(Request zapros) {
    _root = _hosts_and_root.find(zapros.getHost())->second;

}

void Response::autoindexOn()
{
	DIR *dir;
	struct dirent *current;
	std::string body;
	std::string relativePath = ".";
	std::cout << "relative path " << relativePath <<'\n';

    chdir(_root.c_str());
    std::cout << "ROOT autoindex" << _root << std::endl;
    //chdir("/Users/iserzhan/WebServ/site");
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

        char * buffer = new char [length];
        in.read(buffer, length);
        answer_body.append(buffer, in.gcount());//надо добавить проверки
    }
    in.close();     // закрываем файл
}

std::string error_404(){
    std::string error_answer("HTTP/1.1 404 Not Found\r\nServer: my_webserver\n");
    error_answer += current_date();
    error_answer += "Content-Type: text/html\n"
                    "Content-Length: 153\n"
                    "Connection: close\n"
                    "\n"
                    "<html>\n"
                    "<head><title>404 Not Found</title></head>\n"
                    "<body>\n"
                    "<center><h1>404 Not Found</h1></center>\n"
                    "<hr><center>My_webserver</center>\n"
                    "</body>\n"
                    "</html>";
    return error_answer;
}

std::string error_400(){
    std::string error_answer("HTTP/1.1 400 Bad Request\nServer: my_webserver\n");
    error_answer += current_date();
    error_answer += "Content-Type: text/html\n"
                    "Content-Length: 157\n"
                    "Connection: close\n" //надо закрыть соединение после такого ответа
                    "\n"
                    "<html>\n"
                    "<head><title>400 Bad Request</title></head>\n"
                    "<body>\n"
                    "<center><h1>400 Bad Request</h1></center>\n"
                    "<hr><center>My_webserver</center>\n"
                    "</body>\n"
                    "</html>";
    return error_answer;
}

std::string error_403(){
    std::string error_answer("HTTP/1.1 400 Forbidden\nServer: my_webserver\n");
    error_answer += current_date();
    error_answer += "Content-Type: text/html\n"
                    "Content-Length: 153\n"
                    "Connection: close\n" //надо закрыть соединение после такого ответа
                    "\n"
                    "<html>\n"
                    "<head><title>403 Forbidden</title></head>\n"
                    "<body>\n"
                    "<center><h1>403 Forbidden</h1></center>\n"
                    "<hr><center>My_webserver</center>\n"
                    "</body>\n"
                    "</html>";
    return error_answer;
}

std::string content_type(std::string const &file_path) {
    int extentionOfScript = 0;
    if (file_path.find(".html") != std::string::npos)
        return ("text/html");
    else if (file_path.find(".jpg") != std::string::npos)
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

void Response::make_headers(Request & zapros)
{
        if(_code == 200)
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
            _answer += "\r\n";
            _answer += _answer_body;
        }
}

void Response::setValues(Request zapros)
{
    if (zapros.getResourseName() == "/")
        _file_path = _root + "/index.html";
    else
        _file_path = _root + zapros.getResourseName() + "/index.html";
    _content_type = content_type(_file_path);
}

void Response::resetValues(Request & zapros)
{
    _content_type = zapros.getContentType();
    _answer_body = zapros.getAnswerBody();
}

void Response::check_location(Request zapros, std::vector<Server>& servers)
{
    _code = 200;
    Server serv;
    std::string port;
    int size = zapros.getHost().size();

    int f = zapros.getHost().find(":", 0);
    port = zapros.getHost().substr(f + 1, size - (f + 1));
    for (int i = 0; i < servers.size(); i++)
    { 
        if (servers[i]._port == port)
        {
            for (int j = 0; j < servers[i]._locations.size(); j++)
            {
                std::cout << "PATH: " << servers[i]._locations[j]._path << std::endl;
                std::cout << "Resourse_name: " << zapros.getResourseName() << std::endl;
                if(servers[i]._locations[j]._path == zapros.getResourseName())
                {
                    std::cout << "FILE_PATH: " << _file_path << std::endl;
                    int a = 0;
                    for (; a < servers[i]._locations[j]._allowed_methods.size(); a++)
                    {
                        if(zapros.getMethod() == servers[i]._locations[j]._allowed_methods[a])
                            break;
                    }
                    if (a == servers[i]._locations[j]._allowed_methods.size()) // doshla do konca vectora
                    {
                        _answer = error_400(); // poka net 405
                        _code = 405;
                    }
                    else if(servers[i]._locations[j]._autoindex)
                        autoindexOn();
                    return; //naideno location
                }
            }
        }
    }
    _code = 404;
    return;
}

void Response::make_get_response(Request zapros, std::vector<Server>& servers) {
    check_location(zapros, servers);
    if (_code == 200)
    {
        if (check_file_location(_file_path) == -404)
        {
            _answer = error_404();
            _code = 404;
            return;
        }
        else
            file_read(_file_path, _answer_body);
        make_headers(zapros);
    }
}

void Response::make_delete_response(Request zapros)
{
    _code = 200;
     if (check_file_location(_file_path) == 0)
     {
         if (!remove(_file_path.c_str()))
            _answer = error_403();
     }
     else 
     {
         _answer = error_404();
            _code = 404;
        return;
     }
     make_headers(zapros);
}

void Response::make_post_response(Request & zapros, std::string & root)
{
    Cgi c;
    if (check_file_location(_file_path) == -404)
    {
        _answer = error_404();
        _code = 404;
        return;
    }
    c.execute_cgi(zapros, root);
    resetValues(zapros);
    make_headers(zapros);
}

std::string get_file_name(const char *buf){
    int i = 4;
    std::string file_name;
    while (buf[i] != ' '){
        file_name += buf[i];
        i++;
    }
    return file_name;
}

void Response::choose_method(Request & zapros, std::vector<Server>& servers)
{
    ErrorsValue();
    find_root(zapros);
    std::cout << "ROOT < " << _root << std::endl;
    setValues(zapros);
    if (zapros.getMethod() == "GET")
        make_get_response(zapros, servers);
    else if (zapros.getMethod() == "DELETE")
        make_delete_response(zapros);
    else if (zapros.getMethod() == "POST")
        make_post_response(zapros, _root);
    else 
        _answer = error_400();
}