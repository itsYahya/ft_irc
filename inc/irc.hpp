#ifndef IRC_HPP
#define IRC_HPP

#include "server.hpp"
#include "client.hpp"

int							parse_arguments(int ac, char **av, server &server);
std::vector<std::string>	split(std::string str, char sep);

#endif

