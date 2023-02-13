#include <iostream>
#include "irc.hpp"

int main(int ac, char **av){
	server server;

	if (ac == 2 || ac == 3){
		if (parse_arguments(ac, av, server)){
			std::cerr << "error !" << std::endl;
			return (1);
		}
	}else
		std::cerr << "wrong number of arguments" << std::endl;
	return (1);
}