#include "command.hpp"
#include "helper.hpp"

std::string	command::botList(client &c){
	std::string info = "300 * :";
	info += c.getnickName() + " :BOT :Commands to use with it. (BOT CMD)\n";
	info += "300 * :HELP :List of all the commands available in the server\n";
	info += "300 * :SESS :The time you spend on the server\n";
	info += "300 * :CHECKNICK :Check if a given nick name is free\n";
	info += "300 * :TIME :This command tells the date and time of based on the server region\n";
	info += "300 * :BOT :-------------------------------------\n";
	return (info);
}

std::string	command::cmdList(client &c){
	std::string	info = "300 * :";
	info +=  c.getnickName() + " :BOT :HELP :Commands available on this IRC server\n";
	info += "300 * :PRIVMSG\n";
	info += "300 * :JOIN\n";
	info += "300 * :PART\n";
	info += "300 * :LIST\n";
	info += "300 * :KICK\n";
	info += "300 * :QUIT\n";
	info += "300 * :HELP :-------------------------------------\n";
	return (info);
}

std::string	command::sessionTime(client &c, int fd){
	std::string msg = "300 * :" + c.getnickName() + " :SESS :Your log time on the server is : ";
	msg += helper::timeToString(c.getSessionTime()) + "\n";
	::send(fd, msg.c_str(), msg.length(), 0);
	return (msg);
}

void	command::checkNick(int fd, std::string nick){
	std::string msg;
	int			client;

	if (nick.empty())
		msg = "300 * :BOT :CHECKNICK (ERROR) No nick given\n";
	else{
		client = dbManager::searchClient(nick);
		msg = "300 * :BOT " + nick + " :this nickname is ";
		if (client == -1) msg += "free to use\n";
		else msg += "already in use\n";
	}
	::send(fd, msg.c_str(), msg.length(), 0);
}

void	command::timeCmd(int fd){
	std::string msg = "300 * :BOT :Time ";
	msg += helper::nowTime();
	::send(fd, msg.c_str(), msg.length(), 0);
}

void	command::botHandler(client &c, int fd){
	std::string					&list = c.getList();
	int							&index = c.getWindex();
	std::vector<std::string>	res;

	if (body.empty()){
		list = botList(c);
		c.setWriteState();
		index = 0;
		server::write(fd, c);
	}
	else{
		res = helper::split_(body.c_str(), ' ');
		switch (search_cmd(helper::capitalize(res[0])))
		{
			case BOT_HELP:
				list = cmdList(c);
				c.setWriteState();
				index = 0;
				server::write(fd, c);
				break;
			case BOT_SESS:
				sessionTime(c, fd);
				break;
			case BOT_CHECKNICK:
				checkNick(fd, res[1]);
				break;
			case BOT_TIME:
				timeCmd(fd);
				break;
			default:
				std::string msg = ":" + server::getShost() + " 421 ";
				msg += c.getnickName() + " " + body + " :It's not a BOT command\n";
				send(c.getfdClient(), msg.c_str(), msg.length(), 0);
		}
	}
}
