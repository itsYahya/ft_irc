#ifndef DBMANAGER_HPP
# define DBMANAGER_HPP

# include <map>
# include <vector>
# include "client.hpp"
# include "channel.hpp"

class dbManager
{
	private:
		static dbManager* instance;
		static std::map<std::string, channel> channels;
		static std::map<std::string, client> clients;
		dbManager();
	public:

		~dbManager();
		static	dbManager&	getInstance();
		
		static	bool		insertClient(client cl);
		static	client&		searchClient(std::string nick);
		static	client&		searchClient(int fd);
		static	bool		deleteClient(std::string nick);

		static	bool		insertChannel(channel ch);
		static	channel&	searchChannel(std::string nameChannel);
		static	bool		joinClientChannel(std::string nameChannel, std::string nick);
};

#endif