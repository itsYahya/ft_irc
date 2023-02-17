#include "command.hpp"
#include <iostream>
#include <sstream>

command::command(const char *buffer){
	this->buffer = buffer;
	std::stringstream stream(buffer);
	std::getline(stream, name, ' ');
	std::istreambuf_iterator<char> eos;
	std::string ss(std::istreambuf_iterator<char>(stream), eos);
	body = ss;
}

command::~command(){}