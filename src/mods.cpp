#include "channel.hpp"
#include "client.hpp"
#include "command.hpp"
#include "helper.hpp"
#include "dbManager.hpp"

bool	command::channelMode(client &c, channel &ch, std::string::iterator iter, std::string &body){
	std::string		msg, rest;
	long			len;
	helper::vector	result;

	if (*iter == 'm'){
		msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +m\r\n";
		ch.moderate(msg);
	}
	else if (*iter == 't'){
		msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +t\r\n";
		ch.protecTopic(msg);
	}
	else {
		if (*(iter + 1) && *(iter + 1) != ' ')
			return false;
		rest = std::string(iter + 1, body.end());
		result = helper::split(rest, ' ');
		if (result.size() == 0)
			return (sendErrMsg(c.getfdClient(), c.getnickName(), std::string("MODE +") + (*iter), " :Not enough parameters\r\n", " 461 "), false);
		if (*iter == 'l'){
			len = helper::strtol(result[0]);
			if (len <= 0)
				return (false);
			msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +l " + helper::itos(len) + "\r\n";
			ch.setLimit(len, msg);
		}else {
			msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +k " + result[0] + "\r\n";
			
		}
		return (false);
	}
	return (true);
}

void	command::handlModes(client &c, channel &ch, std::string &body){
	std::string::iterator iter = body.begin();
	(void)ch;
	for (; iter != body.end() && (*iter == ' '); iter++)
		;
	for (; iter != body.end(); iter++){
		if (*iter == ' ') break;
		if (*iter == '+') continue;
		if (*iter == 't' || *iter == 'm' || *iter == 'l' || *iter == 'k')
			channelMode(c, ch, iter, body);
		else if (*iter == 'v' || *iter == 'o')
			std::cout << "hello there2" << std::endl;// clientMode(c, ch, *iter);
		else
			sendErrMsg(c.getfdClient(), c.getnickName(), std::string() + *iter, " :is unknown mode char to me\r\n", " 472 ");
	}
}

void	command::modeCmd(client &c, int fd){
	std::vector<std::string>	res;
	dbManager::iterator_channel	cit;

	if (body.empty())
		sendErrMsg(fd, c.getnickName(), "MODE", " :Not enough parameters\r\n", " 461 ");
	else{
		res = helper::split_(body.c_str(), ' ');
		cit = dbManager::searchChannel(res[0]);
		if (dbManager::isEndChannelIter(cit))
			sendErrMsg(fd, c.getnickName(), res[0], " :No such channel\r\n", " 403 ");
		else if (res.size() == 1){

		}else{
			if (c.getmode(res[0]) != OP_CLIENT)
				sendErrMsg(fd, c.getnickName(), res[0], " :You're not channel operator\r\n", " 482 ");
			else
				handlModes(c, cit->second, res[1]);
		}
	}
}