#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <dirent.h>
#include "Server.hpp"

#include <sstream>
#include <fstream>

class Response
{
    private:

		int 						_autoindex;
	std::string                 _zapros;
        std::string                 _answer;
        std::string                 _answer_body;
        std::string                 _content_type;
        std::string                 _status;
        int                         _code;
        std::map<int, std::string>	_errors;
        std::string                 _file_path;
        std::string                 _root;
        std::map<std::string, std::string>						_hosts_and_root;
        std::map<std::string, std::map<int, std::string> >		_default_errors;
        void                        find_root(Request zapros);
    public:
        void                        fill_hosts_and_root(std::vector<Server>& servers);
        void                        choose_method(Request & zapros);
        void                        make_get_response(Request zapros);
        void                        make_delete_response(Request zapros);
        void                        make_response(Request zapros);
        void                        make_headers(Request & zapros);
        void                        make_post_response(Request & zapros);
        void                        resetValues(Request & zapros);
        void                        setValues(Request zapros);
        void                        setFilePath(Request zapros) {}
        std::string                 getAnswer() const {return this->_answer;}
        std::string		            getStatus(int code);
        std::string		            getFilePath() {return _file_path;}
        void                        ErrorsValue();
        void 						autoindexOn();
        std::string					error_404(std::string const &key /* 127.0.0.1:8081*/);

        Response() {}
        ~Response() {}
};

#endif