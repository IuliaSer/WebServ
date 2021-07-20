#include <iostream>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include <errno.h>

int main()
{
    errno = 0;

    std::string file_path("/Users/iserzhan/WebServ/site/python.py");
    // std::string buf;
    // int status;
    // const char *argv[2] = {"/usr/local/bin/python3", NULL};
    //create2darray(zapros);
    // int fd, fd2;
    // if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
    //     return 0;
    // if ((fd2 = open(std::string(root + "/tmp.bla").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
    //     return 0;
    //pid_t child_id = fork();
    // if (!child_id)
    // {
    //     dup2(fd, 0);
    //     dup2(fd2, 1);
    //     //chdir("/Users/iserzhan/");
        if (execve(file_path.c_str(), NULL, NULL) == -1) {
            std::cout << "hi\n";
            perror(": ");
            // close(fd);
            // close(fd2);
            return 0;
        }
    // }
    // else 
    // {
    //     waitpid(child_id, &status, 0);
    //     close(fd);
    //     close(fd2);
    //     fd = open(file_path.c_str(), O_RDONLY);
    //     // if (write_to_buf(fd) == -1)
    //     // {
    //     //     close(fd);
    //     //     //unlink(std::string(root + "/tmp.bla").c_str());
    //     //     //delete file_path;
    //     //     return 0;
    //     // }
    //     // _answer = std::string(buf);
    //     //close(fd);
    //     //unlink(std::string(root + "/tmp.bla").c_str());
    //     //parseCGI(zapros);
    // }
    return 0;  
}