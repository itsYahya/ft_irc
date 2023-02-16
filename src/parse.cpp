
#include "irc.hpp"
#include <iostream>
#include <ctype.h>


static int	parse_port(char *av, int &port){
	for (int index = 0; av[index]; index++){
		if (!std::isdigit(av[index]))
			return (1);
	}
	port = std::atoi(av);
	return (port <= 0);
}

int	parse_arguments(int ac, char **av, server &server){
	if (parse_port(av[1], server.getport()))
		return (1);
	if (ac == 3)
		server.setpassword(std::string(av[2]));
	return (0);
}