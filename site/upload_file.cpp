#include <iostream>
#include <string>
#include <fstream>

std::string extract_data(std::string key, std::string value_end, std::string &text)
{
    size_t pos = text.find(key) + key.length();
    size_t end = text.find(value_end);
    std::string result(text.substr(pos, end - pos));
    return result;
}

int check_if_file_already_exists(std::string filename){
    std::ifstream check(filename);
    if (check.is_open())
    {
        check.close();
        return -1;
    }
    return 0;
}

void    error_on_file_upload(std::string check){
    std::cout << "Content-type: text/html\r\n\r\n"
                 "<!DOCTYPE html>\n"
                 " <head>\n"
                 "  <meta charset=\"utf-8\">\n"
                 "  <title>Ошибка при загрузке файла </title>\n"
                 " </head>\n"
                 " <body>\n"
                 "  <div id=\"centerLayer\">\n"
                 " Файл уже существует\n"
                 "  </div>\n"
                 " </body>\n"
                 "</html> ";
}

int main(int argc, char **argv, char **envp)
{
//    std::string length(envp[1]);
    std::string content_type(envp[2]);
    std::string root(envp[10]);
    std::string body(argv[1]);

    root.erase(0, 16);
//    size_t pos = content_type.find("boundary=") + 9;
//    size_t end_of_delimetr = content_type.find("\r\n", pos);
//    std::string delimetr(content_type.substr(pos, end_of_delimetr - pos));
    std::string delimetr(extract_data("boundary=", "\r\n", content_type));
    delimetr = "--" + delimetr;
//    size_t start_of_body = body.find(delimetr) + delimetr.length();
//    size_t pos_of_filename = body.find("filename=", start_of_body) + 9;
//    size_t end_of_filename = body.find("\r\n", pos_of_filename);
//    std::string filename(body.substr(pos_of_filename, end_of_filename - pos_of_filename));
    std::string filename(extract_data("filename=\"", "\"\r\n", body));
    if (check_if_file_already_exists(root + "/" + filename) == -1)
    {
        error_on_file_upload("");
        return 0;
    }
    size_t content_type_start = body.find("Content-Type", delimetr.length());
    size_t start_of_data = body.find("\r\n", content_type_start) + 4;
    size_t end_of_data = body.find(delimetr, start_of_data);
    std::ofstream new_file(root + "/" + filename);
    if (new_file.is_open())
    {
        new_file << body.substr(start_of_data, end_of_data - start_of_data);
        std::cout << "Content-type: text/html\r\n"
                     "<!DOCTYPE html>\n"
                     " <head>\n"
                     "  <meta charset=\"utf-8\">\n"
                     "  <title>Файл успешно загружен </title>\n"
                     " </head>\n"
                     " <body>\n"
                     "  <div id=\"centerLayer\">\n"
                     " Файл уже существует\n"
                     "  </div>\n"
                     " </body>\n"
                     "</html> ";
        new_file.close();
        return 0;
    }
    error_on_file_upload("");
    return 0;
}

