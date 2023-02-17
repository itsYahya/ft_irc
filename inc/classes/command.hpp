#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

class command{
	private:
		std::string	name;
		std::string	body;
		const char	*buffer; 

	public:
		command(const char *buffer);
		~command();

};

#endif