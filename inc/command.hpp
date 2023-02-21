#ifndef COMMAND_HPP
#define COMMAND_HPP

# include <string>
# include <map>
# include <iostream>
# include "dbManager.hpp"
# include "exceptions.hpp"

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

		void	insertNickname(dbManager& db, std::string name, client& cl);
	public:
		command(const char *buffer);
		~command();
		static void	init_cmds();
		static int	search_cmd(std::string &name);
		void	switch_cmd( const command &cmd, client& cl, dbManager& db);
		friend std::ostream	&operator<<(std::ostream &o, const command &cmd);
};

#endif