bool CgiTester::executeCgiScript() {
    std::string pathToExecutableFile = _location->root + "/" + _location->exec;
    const char *argvForExecutableFile[2] = {NULL, NULL};

    int status;

    if ((_fd = open(_putFilePath->c_str(), O_RDONLY)) < 0)
        return false;
    int fd = open(std::string(_location->root + "/tmp.bla").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
        return false;

    pid_t idChildProcess = fork();
    if (!idChildProcess) 
    {
        dup2(_fd, 0);
        dup2(fd, 1);
        chdir(std::string(_location->root).c_str());
        if (execve(pathToExecutableFile.c_str(), (char *const *) argvForExecutableFile, _env) == -1) {
            close(fd);
            close(_fd);
            freeMemory(_env);
            return false;
        }

    } 
    else
     {
        waitpid(idChildProcess, &status, 0);
        close(fd);
        close(_fd);
        fd = open(std::string(_location->root + "/tmp.bla").c_str(), O_RDONLY);
        if (writeFileToBuffer(fd, &_responseBody) == -1)
        {
            close(fd);
            unlink(std::string(_location->root + "/tmp.bla").c_str());
            freeMemory(_env);
            delete _putFilePath;
            return false;
        }
        close(fd);
        unlink(std::string(_location->root + "/tmp.bla").c_str());
        parseCGI();
    }
    freeMemory(_env);
    delete _putFilePath;
    return true;
}