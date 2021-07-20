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

int check_file_location(std::string const &get_file){
    std::string path(ROOT);
    path += get_file;
    struct stat fl_stat;
    if (stat(path.c_str(), &fl_stat) != 0)
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
    if (file_path.find(".html") != std::string::npos)
        return ("text/html\n");
    else if (file_path.find(".jpg") != std::string::npos)
        return ("image/jpeg\n");
    else if (file_path.find(".css") != std::string::npos)
        return ("text/css\n");
    else
        return ("");
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

void Response::make_headers(Request zapros)
{
        if(_code == 200)
        {
            _answer = "HTTP/1.1 " + std::to_string(_code) + " " + getStatus(_code) + "\r\n";
            int length = static_cast<int>(_answer_body.length());
            char buffer[1000];
            sprintf(buffer, "%d", length);
            std::string length_string(buffer);
            _answer += "Server: my_webserver";
            _answer += current_date();
            _answer += "Content-Type ";
            _answer += _content_type;
            _answer += "\r\n";
            _answer += "Content-Length: ";
            _answer += length_string;
            _answer += "\r\n";
            _answer += _answer_body;
        }
}

void Response::setValues(Request zapros)
{
    _file_path = ROOT;
    _file_path += zapros.getResourseName();
    _content_type = content_type(_file_path);
    file_read(getFilePath(), _answer_body);
}

void Response::resetValues(Request zapros)
{
    _content_type = zapros.getContentType();
    _answer_body = zapros.getAnswerBody();
}

void Response::make_get_response(Request zapros) {
    _file_path = ROOT;
    _code = 200;
    _file_path += zapros.getResourseName();
    file_read(_file_path, _answer_body);
    if (zapros.getResourseName() == "/")
        file_read("/Users/iserzhan/WebServ/site/index.html", _answer_body);
    else
    {
        if (check_file_location(zapros.getResourseName()) == -404)
        {
            _answer = error_404();
            _code = 404;
        }
    }
    make_headers(zapros);
}

void Response::make_delete_response(Request zapros)
{
    _code = 200;
    _file_path = ROOT;
     if (check_file_location(zapros.getResourseName()) == 0)
     {
         _file_path += zapros.getResourseName();
         if (!remove(_file_path.c_str()))
            _answer = error_403();
     }
     else 
     {
         _answer = error_404();
            _code = 404;
     }
     make_headers(zapros);
}

void Response::make_post_response(Request zapros)
{
    Cgi c;
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

void Response::choose_method(Request zapros)
{
    ErrorsValue();
    setValues(zapros);
    if (zapros.getMethod() == "GET")
        make_get_response(zapros);
    else if (zapros.getMethod() == "DELETE")
        make_delete_response(zapros);
    else if (zapros.getMethod() == "POST")
        make_post_response(zapros);     
}