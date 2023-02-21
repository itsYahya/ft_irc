#include "command.hpp"
#include "server.hpp"
#include "helper.hpp"

std::map<std::string, int> command::cmds;

command::command(const char *buffer){
	std::vector<std::string>	res;
	res = helper::split_(buffer, ' ');
	name = res[0];
	body = res[1];
	type = search_cmd(name);
	this->buffer = buffer;
}

command::~command(){}

void	command::init_cmds(){
	cmds.insert(std::pair<std::string, int>("PASS", PASS));
	cmds.insert(std::pair<std::string, int>("NICK", NICK));
	cmds.insert(std::pair<std::string, int>("USER", USER));
	cmds.insert(std::pair<std::string, int>("PRIVMSG", PRIVMSG));
	cmds.insert(std::pair<std::string, int>("PART", PART));
	cmds.insert(std::pair<std::string, int>("JOIN", JOIN));
	cmds.insert(std::pair<std::string, int>("EXIT", EXIT));
	cmds.insert(std::pair<std::string, int>("LIST", LIST));
	cmds.insert(std::pair<std::string, int>("MODE", MODE));
	cmds.insert(std::pair<std::string, int>("KICK", KICK));
	cmds.insert(std::pair<std::string, int>("DCC", DCC));
	cmds.insert(std::pair<std::string, int>("PING", PING));
}

int		command::search_cmd(std::string &name){
	std::map<std::string, int>::iterator iter = cmds.find(name);
	if (iter == cmds.end())
		return (WRONG);
	return (iter->second);
}

void	command::switch_cmd(const command &cmd, int fd, dbManager& db)
{	(void) fd;
	(void) db;
	switch(cmd.type)
	{
		case PASS:
			std::cout << cmd;
			break;
		case NICK:
			insertNickname(db, body, fd);
			break;
		case USER:
			break;
		case PRIVMSG:
			break;
		case PART:
		case JOIN:
		default :
			std::cout << "command wrong\n";
	}
}

std::ostream	&operator<<(std::ostream &out, const command &cmd){
	out << cmd.name << " " << cmd.type << " " << cmd.body;
	return (out);
}

void	command::insertNickname(dbManager& db, std::string name, int s)
{
	if (!name.empty())
	{
		std::string oldname = db.searchClient(s);
		if(!oldname.empty())
			db.deleteClient(oldname);
		db.insertClient(name, s);
	}
}