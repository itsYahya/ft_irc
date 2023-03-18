#include "channel.hpp"
#include "client.hpp"
#include "command.hpp"
#include "helper.hpp"
#include "dbManager.hpp"

helper::vector	command::parceModes(client &c, std::string &body, std::string::iterator iter){
	std::string 	str;
	helper::vector	res;

	if (*(iter + 1) && *(iter + 1) != ' ')
			return res;
	str = std::string(iter + 1, body.end());
	res = helper::split(str, ' ');
	if (res.size() == 0)
		return (sendErrMsg(c.getfdClient(), c.getnickName(), std::string("MODE +") + (*iter), " :Not enough parameters\r\n", " 461 "), res);
	return (res);
}

bool	command::channelMode(client &c, channel &ch, std::string::iterator iter, std::string &body){
	std::string		msg;
	long			len;
	helper::vector	res;

	if (*iter == 'm'){
		msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +m\r\n";
		ch.moderate(msg);
	}
	else if (*iter == 't'){
		msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +t\r\n";
		ch.protecTopic(msg);
	}
	else if (*iter == 'n'){
		msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +n\r\n";
		ch.setNoExternal();
		ch.notifi(msg);
	}
	else {
		res = parceModes(c, body, iter);
		if (res.size() > 0 && *iter == 'l'){
			len = helper::strtol(res[0]);
			if (len <= 0)
				return (false);
			msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +l " + helper::itos(len) + "\r\n";
			ch.setLimit(len, msg);
		}else if (res.size() > 0) {
			msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +k " + res[0] + "\r\n";
			ch.setKey(res[0], msg);
		}
		return (false);
	}
	return (true);
}

bool	command::clientMode(client &c, channel &ch, std::string::iterator iter, std::string &body){
	helper::vector	res;
	std::string		msg;
	int				fd;
	t_mode			mode = NONE;

	res = parceModes(c, body, iter);
	if (res.size() == 0) return (false);
	fd = dbManager::searchClient(res[0]);
	if (fd < 0)
		sendErrMsg(c.getfdClient(), c.getnickName(), res[0], " :No such nick/channel\r\n", " 401 ");
	else{
		client &cl = server::getClientByFd(fd);
		mode = cl.getmode(ch.getNameChannel());
		msg = c.getClinetFullname() + "MODE " + ch.getNameChannel() + " +" + *iter + " " + res[0] + "\r\n";
		if (mode != NONE && *iter == 'o')
			cl.setmode(ch.getNameChannel(), OP_CLIENT);
		else if (mode != NONE && mode != OP_CLIENT)
			cl.setmode(ch.getNameChannel(), V_CLIENT);
	}
	if (mode == NONE)
		sendErrMsg(c.getfdClient(), c.getnickName(), res[0], " :They aren't on that channel\r\n", " 441 ");
	else
		ch.notifi(msg);
	return (false);
}

void	command::handlModes(client &c, channel &ch, std::string &body){
	std::string::iterator iter = body.begin();
	(void)ch;
	for (; iter != body.end() && (*iter == ' '); iter++)
		;
	for (; iter != body.end(); iter++){
		if (*iter == ' ') break;
		if (*iter == '+') continue;
		if (*iter == 't' || *iter == 'm'
			|| *iter == 'l' || *iter == 'k' || *iter == 'n'){
			if (!channelMode(c, ch, iter, body)) return ;
		}
		else if (*iter == 'v' || *iter == 'o'){
			if (!clientMode(c, ch, iter, body)) return ;
		}
		else if (*iter != 's')
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