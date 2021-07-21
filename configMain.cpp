#include "Config.hpp"
#include <unistd.h>
using namespace std;

//// launch without flags: clang++ configMain.cpp Config.cpp Server.cpp Location.cpp Utils.cpp -o compiled

// clang++ -Wall -Wextra -Werror -std=c++98 configMain.cpp Config.cpp Server.cpp Location.cpp Utils.cpp -o compiled

int main() {

	// todo try catch
	Config conf("test.conf");

	conf.parseConfig();
	std::cout << "HERE\n";
//	sleep(10);
	return (0);
}