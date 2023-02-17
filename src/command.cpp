#include "command.hpp"
#include <iostream>
#include <sstream>
#include "server.hpp"

std::map<std::string, int> command::cmds;

command::command(const char *buffer){
	this->buffer = buffer;
	std::stringstream stream(buffer);
	std::getline(stream, name, ' ');
	std::istreambuf_iterator<char> eos;
	std::string ss(std::istreambuf_iterator<char>(stream), eos);
	body = ss;
	type = search_cmd(name);
}

command::~command(){}

void	command::init_cmds(){
	cmds.insert(std::pair<std::string, int>("PASS", PASS));
	cmds.insert(std::pair<std::string, int>("NICK", NICK));
	cmds.insert(std::pair<std::string, int>("USER", USER));
}

int		command::search_cmd(std::string &name){
	std::map<std::string, int>::iterator iter = cmds.find(name);
	if (iter == cmds.end())
		return (ANY);
	return (iter->second);
}

std::ostream	&operator<<(std::ostream &out, const command &cmd){
	out << cmd.name << " " << cmd.type;
	return (out);
}