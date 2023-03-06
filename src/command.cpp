#include "command.hpp"
#include "server.hpp"
#include "helper.hpp"
#include "exceptions.hpp"

std::map<std::string, int> command::cmds;

command::command(const char *buffer){
	std::vector<std::string>	res;
	res = helper::split_(buffer, ' ');
	name = helper::capitalize(res[0]);
	body = helper::capitalize(res[1]);
	type = search_cmd(name);
	this->buffer = buffer;
}

command::~command(){}

void	command::init_cmds(){
	cmds.insert(std::pair<std::string, int>("PASS", CMD_PASS));
	cmds.insert(std::pair<std::string, int>("NICK", CMD_NICK));
	cmds.insert(std::pair<std::string, int>("USER", CMD_USER));
	cmds.insert(std::pair<std::string, int>("PRIVMSG", CMD_PRIVMSG));
	cmds.insert(std::pair<std::string, int>("PART", CMD_PART));
	cmds.insert(std::pair<std::string, int>("JOIN", CMD_JOIN));
	cmds.insert(std::pair<std::string, int>("EXIT", CMD_EXIT));
	cmds.insert(std::pair<std::string, int>("LIST", CMD_LIST));
	cmds.insert(std::pair<std::string, int>("MODE", CMD_MODE));
	cmds.insert(std::pair<std::string, int>("KICK", CMD_KICK));
	cmds.insert(std::pair<std::string, int>("DCC", CMD_DCC));
	cmds.insert(std::pair<std::string, int>("PING", CMD_PING));
	cmds.insert(std::pair<std::string, int>("PONG", CMD_PONG));
}

int		command::search_cmd(std::string &name){
	std::map<std::string, int>::iterator iter = cmds.find(name);
	if (iter == cmds.end())
		return (CMD_WRONG);
	return (iter->second);
}

void	command::prvMsg(client &c, int fd){
	std::string msg = c.getClinetFullname() + name + " " + body + "\n";
	send(fd, msg.c_str(), msg.length(), 0);
}

void	command::sendMsg(dbManager *db, int fd, client &c){
	std::vector<std::string>	res;
	int							client;

	res = helper::split_(body.c_str(), ' ');
	if (res.size() == 0){
		std::string msg = ":localhost 411 " + c.getnickName() + " :No recipient given (PRIVMSG)\n";
		::send(c.getfdClient(), msg.c_str(), msg.length(), 0);
		return ;
	}else if (res.size() == 1){
		std::string msg = ":localhost 412 " + c.getnickName() + " :No text to send\n";
		::send(c.getfdClient(), msg.c_str(), msg.length(), 0);
		return ;
	}
	client = db->searchClient(res[0]);
	if (client > 0)
		prvMsg(c, client);
	else {
		dbManager::iterator_channel iter = db->searchChannel(res[0]);
		if (!dbManager::isEndChannelIter(iter)){
			std::map<std::string, int> &clients = iter->second.getClients();
			std::map<std::string, int>::iterator iter = clients.begin();
			for (; iter != clients.end(); iter++){
				if (iter->second != fd)
					prvMsg(c, iter->second);
			}
		}
		else {
			std::string msg = ":localhost 401 " + c.getnickName() + " " + res[0] + " :No such nick/channel\n";
			send(fd, msg.c_str(), msg.length(), 0);
		}
	}
	
}

void	command::pongCmd(client &c){
	if (body.empty()){
		std::string msg = ":localhost 409 " + c.getnickName() + " :No origin specified\n";
		::send(c.getfdClient(), msg.c_str(), msg.length(), 0);
	}else{
		c.pinged(std::time(NULL));
		c.getPong() = true;
	}
}

void	command::switch_cmd(int fd, dbManager	*db, client &c, std::vector<client> &cls)
{
	switch(type)
	{
		case CMD_PRIVMSG:
			sendMsg(db, fd, c);
			break;
		case CMD_PART:
			partCommand(c, body, *db);
			break;
		case CMD_JOIN:
			joinCommand(c, body, *db, cls);
			break;
		case CMD_LIST:
			sendList(db, fd, c);
			break;
		case CMD_PONG:
			pongCmd(c);
			break;
		case CMD_MODE:
			break;
		default :
			std::string msg = ":localhost 421 ";
			msg += c.getnickName() + " " + name + " :Unknown command\n";
			send(c.getfdClient(), msg.c_str(), msg.length(), 0);
	}
}

std::ostream	&operator<<(std::ostream &out, const command &cmd){
	out << cmd.name << " " << cmd.type << " " << cmd.body;
	return (out);
}

int	command::gettype() const{
	return (type);
}

std::string	command::getname() const{
	return (name);
}

std::string	command::getbody() const{
	return (body);
}

const char	*command::getbuffer() const{
	return (buffer);
}

void	command::joinCommand(client &cl, std::string body, dbManager& db, std::vector<client> &cls)
{
	std::vector<std::string> str = helper::split(body, ' ');
	if (str[0].c_str()[0] == '#' && str[0].length() > 1)
			processJoinPass(cl, str, db, cls);
	else
		db.getInfoInvalid(cl.getfdClient(), cl.getnickName());
}

void	command::insertchannel(std::vector<std::string> body, dbManager &db)
{
	if (body.size() == 2)
	{
		channel ch(body[0], body[1]);
		db.insertChannel(ch);
	}
	else
	{
		channel ch(body[0]);
		db.insertChannel(ch);
	}
}

void	command::joinClChannel(client& cl, std::string body, dbManager& db, std::vector<client> &cls)
{
	db.joinClientChannel(body, cl.getnickName(), cl.getfdClient());
	db.getInfoNewJoin(cl, body);
	db.getInfoListClInChannel(cl, body, cls);
}

void	command::processJoinPass(client &cl, std::vector<std::string> body, dbManager& db, std::vector<client> &cls)
{
	if (!db.srchChannel(body[0]))
	{
		insertchannel(body, db);
		cl.setmode(body[0], OP_CLIENT);
		joinClChannel(cl, body[0], db, cls);
	}
	else
	{
		channel &ch = db.searchChannel(body[0])->second;
		if (!ch.getBannedClient(cl.getHost()) 
				&& (!ch.getIsPasswd() 
				|| (body.size() == 2 && !ch.getPasswd().compare(body[1]))))
		{
			cl.setmode(body[0], SM_CLIENT);
			joinClChannel(cl, body[0], db, cls);
		}
		else
			db.getInfoBan(cl.getfdClient(), cl.getnickName(), body[0]);
	}
}



void	command::partCommand(client &cl, std::string body, dbManager& db)
{
	if (body.c_str()[0] == '#')
	{
		if (db.srchChannel(body))
		{
			db.deleteClientChannel(body, cl.getnickName());
			if (!db.getClients().size())
				db.deleteChannel(body);
		}
	}
	else
		db.getInfoInvalid(cl.getfdClient(), cl.getnickName());
}

void	command::sendList(dbManager *db, int fd, client &c){
	dbManager::iterator_channel		iter;
	dbManager::channels_type		&map_ch = db->getChannels();
	std::string						&list = c.getList();

	iter = map_ch.begin();
	list = channel::getInfosHeader(c.getnickName());
	for (; iter != map_ch.end(); iter++)
		list += iter->second.getInfo(c.getnickName());
	list += channel::getInfosFooter(c.getnickName());
	c.setWriteState();
	c.getWindex() = 0;
	server::write(fd, c);
}