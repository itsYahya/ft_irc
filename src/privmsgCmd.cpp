#include "client.hpp"
#include "channel.hpp"
#include "server.hpp"
#include "command.hpp"
#include "helper.hpp"

void	command::prvMsg(client &c, int fd, std::string nick, std::string body_){
	if (body_.front() != ':') body_.insert(body_.begin(), ':');
	std::string msg = c.getClinetFullname() + name + " " + nick + " " + body_ + "\r\n";
	send(fd, msg.c_str(), msg.length(), 0);
}

void	command::sendMsg(dbManager *db, int fd, client &c, bool err){
	std::vector<std::string>			res;
	std::vector<std::string>::iterator	siter;
	int									client;
	std::string							body_;

	res = helper::split_(body.c_str(), ' ');
	if (res.size() == 0){
		if (err) sendErrMsg(fd, c.getnickName(), "", ":No recipient given (PRIVMSG)\r\n", " 411 ");
		return ;
	}else if (res.size() == 1){
		if (!err) sendErrMsg(fd, c.getnickName(), "", ":No text to send\r\n", " 412 ");
		return ;
	}
	body_ = res[1];
	res = helper::split(res[0], ',');
	siter = res.begin();
	for (; siter != res.end(); siter++){
		client = db->searchClient(*siter);
		if (client > 0)
			prvMsg(c, client, *siter, body_);
		else {
			dbManager::iterator_channel iter = db->searchChannel(*siter);
			if (dbManager::isEndChannelIter(iter)){
				if (err) sendErrMsg(fd, c.getnickName(), *siter, " :No such nick/channel\r\n", " 401 ");
			}
			else {
				if (iter->second.moderated() && c.getmode(iter->first) != OP_CLIENT){
					sendErrMsg(fd, c.getnickName(), *siter, " :Cannot send to channel\r\n", " 404 ");
					continue;
				}
				std::map<std::string, int> &clients = iter->second.getClients();
				std::map<std::string, int>::iterator iter = clients.begin();
				for (; iter != clients.end(); iter++){
					if (iter->second != fd) prvMsg(c, iter->second, *siter, body_);
				}
			}
		}
	}
}