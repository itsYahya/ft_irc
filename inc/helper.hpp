#ifndef HELPER_HPP
#define HELPER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <ctype.h>
#include "server.hpp"

class helper{
	public:
		static std::vector<std::string>	split(std::string str, char sep);
		static std::vector<std::string>	split_(const char *buffer, char sep);
		static int						parse_arguments(int ac, char **av, server &server);
		static std::string				itos(int i);
		static std::string				capitalize(std::string str);
		static std::string				timeToString(time_t time);
};

#endif