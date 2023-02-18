#include "helper.hpp"

static int	parse_port(char *av, int &port){
	for (int index = 0; av[index]; index++){
		if (!std::isdigit(av[index]))
			return (1);
	}
	port = std::atoi(av);
	return (port <= 0);
}

int	helper::parse_arguments(int ac, char **av, server &server){
	if (parse_port(av[1], server.getport()))
		return (1);
	if (ac == 3)
		server.setpassword(std::string(av[2]));
	return (0);
}

std::vector<std::string>	helper::split(std::string str, char sep){
	std::stringstream			stream(str);
	std::string					string;
	std::vector<std::string>	vector;

	while (std::getline(stream, string, sep)){
		if (!string.empty())
			vector.push_back(string);
	}
	return (vector);
}
