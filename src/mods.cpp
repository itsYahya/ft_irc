#include "channel.hpp"
#include "client.hpp"
#include "command.hpp"
#include "helper.hpp"
#include "dbManager.hpp"

void	command::handlModes(client &c, channel &ch, std::string &body){
	std::string::iterator iter = body.begin();
	(void)ch;
	for (; iter != body.end() && (*iter == ' '); iter++)
		;
	for (; iter != body.end(); iter++){
		if (*iter == ' ') break;
		if (*iter == '+') continue;
		if (*iter == 't' || *iter == 'm' || *iter == 'l' || *iter == 'k')
			std::cout << "hello there1" << std::endl;// channelMode(c, ch, *iter);
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