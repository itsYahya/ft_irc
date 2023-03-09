#include "command.hpp"
#include "helper.hpp"


std::string	command::botList(client &c){
	std::string info = ":localhost " + helper::itos(321);
	info += " " + c.getnickName() + " BOT :Commands to use with it. (BOT CMD)\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " HELP :List of all the commands available in the server\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " SESS :The time you spend on the server\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " CHECKNICK :Check if a given nick name is free\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " FAV :Add a favorite list of friends\n";
	info += ":localhost " + helper::itos(323) + " " + c.getnickName() + " End of /LIST\n";
	return (info);
}

std::string	command::cmdList(client &c){
	std::string	info = ":localhost " + helper::itos(321);
	info += " " + c.getnickName() + " HELP :Commands available on this IRC server\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " PRIVMSG\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " JOIN\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " PART\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " LIST\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " KICK\n";
	info += ":localhost " + helper::itos(322) + " " + c.getnickName() + " QUIT\n";
	info += ":localhost " + helper::itos(323) + " " + c.getnickName() + " End of /LIST\n";
	return (info);
}

std::string	command::sessionTime(client &c, int fd){
	std::string msg = "300 * :BOT " + c.getnickName() + " :Your log time on the server is : ";
	msg += helper::timeToString(c.getSessionTime()) + "\n";
	::send(fd, msg.c_str(), msg.length(), 0);
	return (msg);
}

void	command::botHandler(client &c, int fd){
	std::string	&list = c.getList();
	int			&index = c.getWindex();

	if (body.empty()){
		list = botList(c);
		c.setWriteState();
		index = 0;
		server::write(fd, c);
	}
	else{
		switch (search_cmd(body))
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
			default:
				break;
		}
	}
}
