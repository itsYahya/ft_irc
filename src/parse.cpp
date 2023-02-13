
#include "irc.hpp"
#include <iostream>

static int	ft_isdegit(char ch){
	return ((ch <= '9' && ch >= '0'));
}

static int	ft_atoi(char *port){
	int	num = 0;

	for (int index = 0; port[index]; index++){
		num = (num * 10) + (port[index] - '0');
	}
	return (num);
}

static int	parse_port(char *av, int &port){
	for (int index = 0; av[index]; index++){
		if (!ft_isdegit(av[index]))
			return (1);
	}
	port = ft_atoi(av);
	return (port <= 0);
}

int	parse_arguments(int ac, char **av, server &server){
	if (parse_port(av[1], server.port))
		return (1);
	if (ac == 3)
		server.password = std::string(av[2]);
	std::cout << server.password << std::endl;
	return (0);
}