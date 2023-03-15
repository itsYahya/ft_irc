#include "channel.hpp"
#include "client.hpp"
#include "command.hpp"
#include "helper.hpp"
#include "dbManager.hpp"

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
				std::cout << "hello there" << std::endl;
		}
	}
}