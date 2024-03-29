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

std::vector<std::string>	helper::split_(const char *buffer, char sep){
	std::string 				name;
	std::vector<std::string>	res;
	std::stringstream stream(buffer);

	std::getline(stream, name, sep);
	std::istreambuf_iterator<char> eos;
	std::string ss(std::istreambuf_iterator<char>(stream), eos);
	if (!name.empty()) res.push_back(name);
	if (!ss.empty()) res.push_back(ss);
	return (res);
}

std::string	helper::itos(size_t i){
	std::stringstream	stream;
	std::string			string;

	stream << i;
	stream >> string;
	return (string);
}

std::string	helper::capitalize(std::string str){
	std::string::iterator iter = str.begin();

	for (; iter != str.end(); iter++)
		*iter = std::toupper(*iter);
	return (str);
}

std::string	helper::timeToString(time_t time){
	int m, h, s;

	m = time / 60;
	h = m / 60;
	m = m % 60;
	s = time % 60;
	return (helper::itos(h) + "h " + helper::itos(m) + "m " + helper::itos(s) + "s");
}

std::string	helper::nowTime(){
	time_t	time = std::time(NULL);
	std::string str = std::ctime(&time);
	return (str);
}

std::vector<std::string>	helper::splitCmds(std::string buffer, char sep){
	std::vector<std::string>	res;
	std::stringstream			stream(buffer);
	std::string					string;

	while (std::getline(stream, string, sep)){
		if (string.size() > 1 && string.front() == '\n') string.erase(string.begin());
		if (string.size() > 1 && string.back() == '\n') string.erase(string.end() - 1);
		if (!string.empty() && string[0] != '\n') res.push_back(string);
	}
	return (res);
}

long helper::strtol(std::string str){
	std::string::iterator	iter = str.begin();
	long					res = 0;

	for (; iter != str.end() && *iter == ' '; iter++)
		;
	for (; iter != str.end(); iter++){
		if (res < 0 || !std::isdigit(*iter)) return (res);
		res = res * 10 + (*iter - '0');
	}
	return (res);
}
