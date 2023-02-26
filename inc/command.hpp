#ifndef COMMAND_HPP
#define COMMAND_HPP

# include <string>
# include <map>
# include <iostream>
# include "dbManager.hpp"

class server;

typedef enum e_ctype{
	CMD_PASS 	= 1,
	CMD_NICK 	= 2,
	CMD_USER 	= 3,
	CMD_PRIVMSG = 4,
	CMD_PART	= 5,
	CMD_JOIN	= 6,
	CMD_EXIT	= 7,
	CMD_LIST	= 8,
	CMD_MODE	= 9,
	CMD_KICK	= 10,
	CMD_DCC		= 11,
	CMD_PING	= 12,
	CMD_WRONG 	= 0,
}	t_ctype;

class command{
	private:
		int									type;
		std::string							name;
		std::string							body;
		const char							*buffer;
		static std::map<std::string, int>	cmds;
		void								joinCommand(client &cl, std::string body, dbManager& db);
		void								partCommand(client &cl, std::string body, dbManager& db);
	public:
		command(const char *buffer);
		~command();
		static void							init_cmds();
		static int							search_cmd(std::string &name);
		void								switch_cmd( const command &cmd, int fd, dbManager *cl, client &c);
		friend std::ostream					&operator<<(std::ostream &o, const command &cmd);
		int									gettype() const ;
		std::string							getname() const ;
		std::string							getbody() const ;
		const char							*getbuffer() const ;

		void								sendMsg(dbManager *db, int fd, client &c);
		void								prvMsg(std::string sender, int fd);
};

#endif