#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <map>
#include <iostream>

class server;

typedef enum e_ctype{
	PASS = 1,
	NICK = 2,
	USER = 3,

	ANY = 4,
}	t_ctype;

class command{
	private:
		int									type;
		std::string							name;
		std::string							body;
		const char							*buffer;
		static std::map<std::string, int>	cmds;

	public:
		command(const char *buffer);
		~command();
		static void	init_cmds();
		static int	search_cmd(std::string &name);
		
};

#endif