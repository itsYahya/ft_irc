#include "command.hpp"
#include "helper.hpp"

std::string	command::botList(client &c){
	std::string info = "\n300 * :";
	info += c.getnickName() + " :BOT :Commands to use with it. (BOT CMD)\n";
	info += "300 * :HELP :List of all the commands available in the server\n";
	info += "300 * :SESS :The time you spend on the server\n";
	info += "300 * :CHECKNICK :Check if a given nick name is free\n";
	info += "300 * :FAV :Add a favorite list of friends\n\n";
	info += "300 * :BOT :" + c.getnickName() + " End of /LIST\n";
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
	info += "300 * :QUIT\n\n";
	info += "300 * :HELP :" + c.getnickName() + " End of /LIST\n";
	return (info);
}

std::string	command::sessionTime(client &c, int fd){
	std::string msg = "300 * :" + c.getnickName() + " :BOT :Your log time on the server is : ";
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
