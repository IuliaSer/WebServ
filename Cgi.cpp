#include "Cgi.hpp"

int Cgi::write_to_buf(int fd)
{
    int r = 1;
    while (r > 0)
    {
        r = read(fd, _buf, CGI_BUFSIZE);
        if (r == -1)
            return -1;
    }
    return 0;
}

void Cgi::parseCGI(Request zapros)
{
    int i;
    int j = 0;
    i = _answer.find("\r\n\r\n", 0);
    if (i == std::string::npos)
        i = 0;
    j = _answer.find("Status: ", 0);
    if (j > 0)
        zapros.setStatus(_answer.substr(j + 8, 3));
    if ((j = _answer.find("Content type: ", 0)))
        zapros.setContentType(_answer.substr(j + 14, _answer.find("\r\n", j)));
    if (i > 0)
        zapros.setAnswerBody(_answer.substr(i, _answer.size() - i));
}  

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*nstr;
	int		i;
	int		j;
	int		len;

	if (!s1 || !s2)
		return (0);
	len = strlen(s1) + strlen(s2);
	if (!(nstr = (char*)malloc(sizeof(char) * len + 1)))
		return (NULL);
	j = 0;
	i = 0;
	while (s1[j])
		nstr[i++] = s1[j++];
	j = 0;
	while (s2[j])
		nstr[i++] = s2[j++];
	nstr[i] = '\0';
	return (nstr);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	size;
	size_t	i;

	i = 0;
	size = strlen(s);
	if (!(str = (char*)malloc(sizeof(char) * size + 1)))
		return (NULL);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void Cgi::create2darray(Request zapros)
{
    _env = new char *[5 + zapros.getHeaders().size()];
    _env[0] = ft_strdup("AUTH_TYPE=Anonymous");
    _env[1] = ft_strjoin("CONTENT_LENGTH", (zapros.getContentLength().c_str()));
    _env[2] = ft_strjoin("CONTENT_TYPE= ", (zapros.getContentType()).c_str());
    _env[3] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
    // _env[4] = "PATH_INFO=/directory
    // _env[5] = "PATH_TRANSLATED="/Users/anatashi/goinfre/webServ/webServ/YoupiBanane/directory
    // _env[6] = "QUERY_STRING=""
    // _env[7] = "REMOTE_ADDR="127.0.0.1
    // _env[8] = "REMOTE_IDENT=".localhost:8080
    // _env[9] = "REMOTE_USER="
    _env[4] = ft_strjoin("REQUEST_METHOD=", (zapros.getMethod().c_str()));
    //_env[11] = "REQUEST_URI=/directory
    // _env[12] = "SCRIPT_NAME="cgi_tester
    // _env[13] = "SERVER_NAME="for_tester
    // _env[14] = "SERVER_PORT="8080;
    // _env[15] = "SERVER_PROTOCOL=HTTP/1.1";
    // _env[16] = "SERVER_SOFTWARE=webServ";
    _env[5] = NULL;
}

int Cgi::execute_cgi(Request zapros)
{
    std::string root(ROOT);
    std::string file_path;
    std::string buf;
    int status;
    const char *argv[2] = {NULL, NULL};
    file_path += root;
    file_path += zapros.getResourseName();
    create2darray(zapros);
    int fd, fd2;
    if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
        return 0;
    if ((fd2 = open(std::string(root + "/tmp.bla").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
        return 0;
    pid_t child_id = fork();
    if (!child_id)
    {
        dup2(fd, 0);
        dup2(fd2, 1);
        if (execve(file_path.c_str(), (char *const *) argv, _env) == -1) {
            close(fd);
            close(fd2);
            return 0;
        }
    }
    else 
    {
        waitpid(child_id, &status, 0);
        close(fd);
        close(fd2);
        fd = open((root + "/tmp.bla").c_str(), O_RDONLY);
        if (write_to_buf(fd) == -1)
        {
            close(fd);
            //unlink(std::string(root + "/tmp.bla").c_str());
            //delete file_path;
            return 0;
        }
        _answer = std::string(buf);
        close(fd);
        //unlink(std::string(root + "/tmp.bla").c_str());
        //parseCGI(zapros);
    }
    return 0;  
}