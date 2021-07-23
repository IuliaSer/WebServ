#include "main.hpp"
#include <iostream> // std::cout.
#include <string> // std::string, std::to_string.
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

int check_file_location(std::string const &file_path){
    // std::string path(_root);
    // path += get_file;
    struct stat fl_stat;
    if (stat(file_path.c_str(), &fl_stat) != 0)
       return (404);
    //check permission
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
    // _file_path = _root;
    if (zapros.getResourseName() == "/")
        _file_path = _root;
    else
        _file_path = _root + zapros.getResourseName();
    _content_type = content_type(_file_path);
    // file_read(getFilePath(), _answer_body);
}

void Response::resetValues(Request & zapros)
{
    _content_type = zapros.getContentType();
    _answer_body = zapros.getAnswerBody();
}

void Response::make_get_response(Request zapros) {
    // _file_path = _root;
    _code = 200;
    // _file_path = _root + zapros.getResourseName();
    
    if (zapros.getResourseName() == "/") {
        file_read(_root + "/index.html", _answer_body);
        _content_type = content_type(_root + "/index.html");
    }
    else
    {
        if (check_file_location(_file_path) == -404)
        {
            _answer = error_404();
            _code = 404;
            return;
        }
        else
            file_read(_file_path, _answer_body);
    }
    make_headers(zapros);
}

void Response::make_delete_response(Request zapros)
{
    _code = 200;
    // _file_path = _root + zapros.getResourseName();
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

void Response::make_post_response(Request & zapros)
{
    Cgi c;
    if (check_file_location(_file_path) == -404)
    {
        _answer = error_404();
        _code = 404;
        return;
    }
    c.execute_cgi(zapros);
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

void Response::choose_method(Request & zapros)
{
    ErrorsValue();
    find_root(zapros);
    std::cout << "ROOT < " << _root << std::endl;
    setValues(zapros);
    if (zapros.getMethod() == "GET")
        make_get_response(zapros);
    else if (zapros.getMethod() == "DELETE")
        make_delete_response(zapros);
    else if (zapros.getMethod() == "POST")
        make_post_response(zapros);
    else 
        _answer = error_400();
}