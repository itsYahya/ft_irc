#ifndef COMMAND_HPP
#define COMMAND_HPP

# include <string>
# include <map>
# include <iostream>
# include "dbManager.hpp"

class server;

typedef enum e_ctype{
	CMD_PASS 		= 1,
	CMD_NICK 		= 2,
	CMD_USER 		= 3,
	CMD_PRIVMSG 	= 4,
	CMD_PART		= 5,
	CMD_JOIN		= 6,
	CMD_EXIT		= 7,
	CMD_LIST		= 8,
	CMD_MODE		= 9,
	CMD_KICK		= 10,
	CMD_DCC			= 11,
	CMD_PING		= 12,
	CMD_PONG		= 13,
	CMD_QUIT		= 14,
	CMD_BOT			= 15,
	BOT_HELP		= 16,
	BOT_SESS		= 17,
	BOT_CHECKNICK	= 18,
	BOT_TIME		= 19,
	CMD_AWAY		= 20,
	CMD_TOPIC		= 21,
	CMD_NOTICE		= 22,
	CMD_WRONG 		= 0,
}	t_ctype;

class command{
	private:
		int									type;
		std::string							name;
		std::string							body;
		const char							*buffer;
		static std::map<std::string, int>	cmds;
		void 	processJoinPass(client &cl, std::vector<std::string> body, dbManager &db, std::vector<client> &cls);
		void	insertchannel(std::vector<std::string> body, dbManager &db);
		void	joinClChannel(client& cl, std::string body, dbManager& db, std::vector<client> &cls);
	public:
		command(const char *buffer);
		~command();
		static void							init_cmds();
		static int							search_cmd(const std::string &name);
		void								switch_cmd(int fd, dbManager *cl, client &c, std::vector<client> &cls);
		friend std::ostream					&operator<<(std::ostream &o, const command &cmd);
		int									gettype() const ;
		std::string							getname() const ;
		std::string							getbody() const ;
		const char							*getbuffer() const ;
		void								partCommand(client &cl, std::string body, dbManager& db, std::vector<client> &cls);
		void								joinCommand(client &cl, std::string body, dbManager& db, std::vector<client> &cls);
		void 								kickCommand(client &cl, std::string body, dbManager& db, std::vector<client> &cls);
		void								sendMsg(dbManager *db, int fd, client &c, bool err);
		void								prvMsg(client &c, int fd, std::string nick, std::string body_);
		
		void								sendList(dbManager *db, int fd, client &c);

		void								pongCmd(client &c);

		void								botHandler(client &c, int fd);
		std::string							botList(client &c);
		std::string							cmdList(client &c);
		std::string							sessionTime(client &c, int fd);
		void								checkNick(int fd, std::string nick);
		void								timeCmd(int fd);

		void								topiCmd(client &c, int fd);
		void								topicInform(client &c, const std::string &ch, const std::string &topic, dbManager::iterator_channel iter);
		static std::string					sendErrMsg(int fd, const std::string &nick, const std::string &name,
											const std::string &body_, const std::string &code);

		void								modeCmd(client &c, int fd);
		void								handlModes(client &c, channel &ch, std::string &body);
		bool								channelMode(client &c, channel &ch, std::string::iterator iter);
};

#endif