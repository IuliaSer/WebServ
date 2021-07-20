#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
    private:
        std::string                         _buf;
        std::string                         _method;
        std::string	                        _resource_name;
        std::string	                        _protocol_version;
        std::string	                        _body;
        std::string                         _content_type;
        std::string                         _content_length;
        std::string                         _status;
        std::string	                        _answer_body;
        size_t                              _len;
        int                                 _sl;
        int                                 _hd;
        int                                 _bd;
        int                                 _bd_flag;
        std::map<std::string, std::string>  _headers;
    public:
        void        						parse_start_line();
        void        						parse_request(std::string str);
        void        						parse_headers();
        void        						parse_body();
        void        						cut_buf(int flag);
        void        						clean_request();
        void                                setStatus(const std::string  &status) {this->_status = status;}
        void                                setAnswerBody(const std::string  &answer_body) {this->_answer_body = answer_body;}
        void                                setContentType(const std::string  &content_type) {this->_content_type = content_type;}
        std::string			                getResourseName() const {return this->_resource_name;}
        std::string			                getMethod() const {return this->_method;}
        std::string			                getContentType() const {return this->_content_type;}
        std::string			                getContentLength() const {return this->_content_length;}
        std::string			                getStatus() const {return this->_status;}
        std::string			                getAnswerBody() const {return this->_answer_body;}
        std::map<std::string, std::string>  getHeaders() const {return this->_headers;}
        std::string                         getHost() const {return _headers.find("Host")->second;}
        Request()
        {
            _answer_body = "";
        }
        ~Request() {}
};

#endif