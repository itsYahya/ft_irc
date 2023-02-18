#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <map>
#include <iostream>

class server;

typedef enum e_ctype{
	PASS 	= 1,
	NICK 	= 2,
	USER 	= 3,
	PRIVMSG = 4,
	PART	= 5,
	JOIN	= 6,
	EXIT	= 7,
	LIST	= 8,
	MODE	= 9,
	KICK	= 10,
	DCC		= 11,
	PING	= 12,
	WRONG 	= 0,
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
		friend std::ostream	&operator<<(std::ostream &o, const command &cmd);
};

#endif