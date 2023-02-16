#include <iostream>
#include "irc.hpp"

int main(int ac, char **av){
	server server;

	try{
		if (ac == 2 || ac == 3){
			if (parse_arguments(ac, av, server)){
				std::cerr << "error !" << std::endl;
				return (1);
			}
			server.create();
			server.listen();
			server.close();
		}else
			std::cerr << "wrong number of arguments" << std::endl;
	} catch (const std::exception &excp){
		std::cerr << excp.what() << std::endl;
	}
	return (1);
}