//
// Created by Phylicia Melinda on 7/9/21.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "Server.hpp"
#include "Location.hpp"
#include "Utils.hpp"

#include <vector>
#include <iostream>
#include <fstream>

class Server;

class Location;

//struct ConfigErrors{
//	const std::string NO_SEMICOLON = "CONFIG ERROR: Directive must end with a \';\'";
//	const std::string INVALID_BLOCK = "CONFIG ERROR: Block directive's syntax error";
//	const std::string BRACKET_ERROR = "CONFIG ERROR: Unexpected end of file, expecting \"}\"";
//	const std::string NO_ARGUMENT = "CONFIG ERROR: Simple directive have at least one arguments";
//	const std::string INVALID_VALUE = "CONFIG ERROR: Invalid arguments at Simple directive";
//	const std::string INVALID_DIRECtiVE = "CONFIG ERROR: Invalid directive";
//};


// todo Exceptions

class Config {
private:
	// Coplien
	Config();
	Config (const Config& other);
	Config& operator=(const Config& rhs);

	std::vector<Server> _servers;
	std::string config_path;

	void parseServer(std::vector<std::string> lines, uint32_t* endBlockPos);
	void parseLocation(std::vector<std::string> lines, uint32_t* endBlockPos);

	void checkServerInstructions(std::vector<std::string> line, uint32_t endBlockPos);
	void checkLocationInstructions(std::vector<std::string> line, uint32_t endBlockPos);
public:
	Config(const char *config_path);
	virtual ~Config();

//	ConfigErrors configErrors;

	void parseConfig(void);

};


#endif //WEBSERV_CONFIG_HPP
