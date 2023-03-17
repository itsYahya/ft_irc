#include "command.hpp"
#include "client.hpp"
#include "dbManager.hpp"
#include "channel.hpp"
#include "server.hpp"
#include "helper.hpp"

std::string	command::sendErrMsg(int fd, const std::string &nick, const std::string &name,
					const std::string &body_, const std::string &code){
	std::string msg = ":" + server::getShost() + code + nick;
	msg += " " + name + body_;
	::send(fd, msg.c_str(), msg.length(), 0);
	return (msg);
}

void	command::topicInform(client &c, const std::string &ch, const std::string &topic, dbManager::iterator_channel iter){
	std::map<std::string, int> &clients = iter->second.getClients();
	std::map<std::string, int>::iterator it = clients.begin(), end = clients.end();
	std::string msg = c.getClinetFullname() + "TOPIC " + ch + " :" + topic + "\r\n";
	for (; it != end; it++)
		::send(it->second, msg.c_str(), msg.length(), 0);
}

void	command::topiCmd(client &c, int fd){
	std::vector<std::string>	res = helper::split_(body.c_str(), ' ');
	dbManager::iterator_channel	iter;
	t_mode						md;

	if (res.size() == 0){
		sendErrMsg(fd, c.getnickName(), name, " :Not enough parameters\r\n", " 331 ");
		return ;
	}
	iter = dbManager::searchChannel(res[0]);
	if (dbManager::isEndChannelIter(iter))
		sendErrMsg(fd, c.getnickName(), res[0], " :No such channel\r\n", " 403 ");
	else if (res.size() == 1)
		sendErrMsg(fd, c.getnickName(), res[0], " :No topic is set.\r\n", " 331 ");
	else{
		md = c.getmode(res[0]);
		if (md == NONE)
			sendErrMsg(fd, c.getnickName(), res[0], " :You're not on that channel\r\n", " 442 ");
		else if (md != OP_CLIENT && iter->second.topicProtected())
			sendErrMsg(fd, c.getnickName(), res[0], " :You're not an operator on the channel\r\n", " 442 ");
		else{
			if (res[1].front() == ':') res[1].erase(res[1].begin());
			iter->second.seTopic(res[1]);
			topicInform(c, res[0], res[1], iter);
		}
	}
}