#ifndef DBMANAGER_HPP
# define DBMANAGER_HPP

# include <map>
# include <vector>
# include "command.hpp"
# include "client.hpp"
# include "channel.hpp"

class dbManager
{
	private:
		static dbManager* instance;
		static std::map<std::string, client> clients;
		static std::map<std::string, std::vector<client> > channels;
		
		dbManager();
	public:
		~dbManager();
		static	dbManager*	getInstance();
		
		static	bool		insertClient(client cl);
		static	bool		searchClient(std::string nick);
		static	bool		deleteClient(std::string nick);

		static	bool		insertChannel(channel ch);
		static	bool		searchChannel(std::string channel);
		static	bool		findClientInChannel(std::string channel, std::string nick);
		static	void		joinClientChannel(std::string channel, std::string nick);
};

#endif