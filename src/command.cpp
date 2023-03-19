#include "command.hpp"
#include "server.hpp"
#include "helper.hpp"
#include "exceptions.hpp"

std::map<std::string, int> command::cmds;

command::command(const char *buffer): body(""){
	std::vector<std::string>	res;
	res = helper::split_(buffer, ' ');
	name = res[0];
	if (res.size() == 2) body = res[1];
	body = res[1];
	if (body.size() && body.front() == ':') body.erase(body.begin());
	type = search_cmd(helper::capitalize(res[0]));
	this->buffer = buffer;
}

command::~command(){}

void	command::init_cmds(){
	cmds.insert(std::pair<std::string, int>("PASS", CMD_PASS));
	cmds.insert(std::pair<std::string, int>("NICK", CMD_NICK));
	cmds.insert(std::pair<std::string, int>("USER", CMD_USER));
	cmds.insert(std::pair<std::string, int>("INVITE", CMD_INVITE));
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
	cmds.insert(std::pair<std::string, int>("QUIT", CMD_QUIT));
	cmds.insert(std::pair<std::string, int>("BOT", CMD_BOT));
	cmds.insert(std::pair<std::string, int>("AWAY", CMD_AWAY));
	cmds.insert(std::pair<std::string, int>("TOPIC", CMD_TOPIC));
	cmds.insert(std::pair<std::string, int>("NOTICE", CMD_NOTICE));
	cmds.insert(std::pair<std::string, int>("HELP", BOT_HELP));
	cmds.insert(std::pair<std::string, int>("SESS", BOT_SESS));
	cmds.insert(std::pair<std::string, int>("TIME", BOT_TIME));
	cmds.insert(std::pair<std::string, int>("CHECKNICK", BOT_CHECKNICK));
}

int		command::search_cmd(const std::string &name){
	std::map<std::string, int>::iterator iter = cmds.find(name);
	if (iter == cmds.end())
		return (CMD_WRONG);
	return (iter->second);
}

void	command::pongCmd(client &c){
	if (body.empty()){
		std::string msg = ":" + server::getShost() + " 409 " + c.getnickName() + " :No origin specified\n";
		::send(c.getfdClient(), msg.c_str(), msg.length(), 0);
	}else{
		c.pinged(std::time(NULL));
		c.getPong() = true;
	}
}

std::string	makeReason(client &c, std::string body){
	std::string reson = body;
	std::string msg = "";
	
	if (reson[0] == ':') reson.erase(reson.begin());
	if (reson.empty()) reson = c.getnickName();
	return (reson);
}

void	command::switch_cmd(int fd, dbManager	*db, client &c, std::vector<client> &cls)
{
	switch(type)
	{
		case CMD_PRIVMSG:
			sendMsg(db, fd, c, true);
			break;
		case CMD_NOTICE:
			sendMsg(db, fd, c, false);
			break;
		case CMD_PART:
			partCommand(c, body, *db, cls);
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
		case CMD_QUIT:
			server::closingLink(makeReason(c, body), c);
			break;
		case CMD_MODE:
			modeCmd(c, fd);
			break;
		case CMD_AWAY:
			break;
		case CMD_PING:
			break;
		case CMD_TOPIC:
			topiCmd(c, fd);
			break;
		case CMD_BOT:
			botHandler(c, fd);
			break;
		case CMD_KICK:
			kickCommand(c, body, *db, cls);
			break;
		case CMD_INVITE:
			inviteCmd(c, body, *db, cls);
			break;
		default :
			std::string msg = ":" + server::getShost() + " 421 ";
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
	if (str.size() == 0)
		sendErrMsg(cl.getfdClient(), cl.getnickName(), "JOIN", " :Not enough parameters\r\n", " 461 ");
	else if (str[0].c_str()[0] == '#' && str[0].length() > 1)
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
	t_mode	mode;

	if (!db.srchChannel(body[0]))
	{
		insertchannel(body, db);
		cl.setmode(body[0], OP_CLIENT);
		joinClChannel(cl, body[0], db, cls);
	}
	else
	{
		channel &ch = db.searchChannel(body[0])->second;
		if (ch.getBannedClient(cl.getHost()) >= 0) 
			db.getInfoBan(cl.getfdClient(), cl.getnickName(), body[0]);
		else if(cl.checkChannel(body[0]))
			db.getInfoPartError(cl, body[0], 500);
		else if (!ch.wantsMore())
			sendErrMsg(cl.getfdClient(), cl.getnickName(), body[0], " :Cannot join channel (+l)\r\n", " 471 ");
		else if (!ch.getIsPasswd() || cl.getInvite(ch.getNameChannel()) >= 0 
				|| (body.size() == 2 && !ch.getPasswd().compare(body[1])))
		{
			mode = (ch.moderated() ? M_CLIENT : SM_CLIENT);
			if (ch.empty()) mode = OP_CLIENT;
			cl.setmode(body[0], mode);
			cl.eraseInvite(ch.getNameChannel());
			joinClChannel(cl, body[0], db, cls);
		}
		else
			db.getInfoPartError(cl, body[0], 475);
	}
}

void	command::partCommand(client &cl, std::string body, dbManager& db, std::vector<client> &cls)
{
	std::vector<std::string> info = helper::split(body, ' ');
	if (info.size() == 0){
		sendErrMsg(cl.getfdClient(), cl.getnickName(), "PART", " :Not enough parameters\r\n", " 461 ");
		return ;
	}
	channel &ch = db.searchChannel(info[0])->second;
	if (info[0].c_str()[0] == '#' && info[0].length() > 1 && db.srchChannel(info[0]))
	{
		if (cl.checkChannel(info[0]))
		{
			db.getInfoPartChannel(cl, info);
			if (ch.clients.size() > 1)
				db.nextClientmode(cl, ch, cls);
			cl.erasemode(info[0]);
			cl.quitChannel(info[0]);
			db.getInfoListClInChannel(cl, ch.getNameChannel(), cls);
			if (ch.clients.size() == 0)
				db.deleteChannel(info[0]);
		}
		else
			db.getInfoPartError(cl, info[0], 442);
	}
	else
		db.getInfoPartError(cl, info[0], 403);
}

void	command::inviteCmd(client &c, std::string body, dbManager& db, std::vector<client> &cls)
{
	std::vector<std::string> str = helper::split(body, ' ');
	if (str.size() == 0){
		sendErrMsg(c.getfdClient(), c.getnickName(), "INVITE", " :Not enough parameters\r\n", " 461 ");
		return ;
	}
	int fd = db.searchClient(str[0]);
	if (fd > 0 && str.size() >= 2)
	{
		client& cl = cls[fd];
		channel& ch = db.searchChannel(str[1])->second;
		if (db.srchChannel(str[1]))
		{
			if(c.getmode(str[1]) == OP_CLIENT)
			{
				cl.setInvite(str[1]);
				if (ch.getBannedClient(cl.getHost()) >= 0)
					ch.deleteBannedClient(cl.getHost());
				std::string info = " join channel: " + str[1]; 
				command::prvMsg(c,cl.getfdClient(), cl.getnickName(), info);
			}
			else
				db.getInfoPartError(c, str[1], 482);
		}
		else
			db.getInfoPartError(c, str[1], 403);
	}
	else
	{
		std::string info =  ":localhost 401 " + str[0] + " " + str[1] + " :no such nick/channel\n";
		send(c.getfdClient(), info.c_str(), info.size(), 0);
	}
		
}

void	command::kickCommand(client &cl, std::string body, dbManager& db, std::vector<client> &cls)
{
	std::vector<std::string> info = helper::split(body, ' ');
	if (info.size() == 0)
		sendErrMsg(cl.getfdClient(), cl.getnickName(), "KICK", " :Not enough parameters\r\n", " 461 ");
	else {
		channel &ch = db.searchChannel(info[0])->second;
		client 	&clK = cls[db.searchClient(info[1])];
		if (db.getInfoKickError(cl, ch, info))
		{
			db.getInfoKickChannel(cl, info);
			db.deleteClientChannel(ch.getNameChannel(), clK.getnickName());
			ch.setBannedClient(clK.getHost());
			clK.erasemode(info[0]);
		}
	}
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