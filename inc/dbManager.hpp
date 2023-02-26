#ifndef DBMANAGER_HPP
# define DBMANAGER_HPP

# include <map>
# include <vector>
# include "client.hpp"
# include "channel.hpp"

class dbManager
{
	private:
		static dbManager* 										instance;
		static std::map<std::string, channel> 					channels;
		static std::map<std::string, int> 						clients;
		typedef std::map<std::string, int>::iterator			iterator_clinet;
		typedef std::map<std::string, channel>::iterator		iterator_channel;
		dbManager();

	public:

		~dbManager();
		static	dbManager&	getInstance();
		
		static	bool		insertClient(std::string name, int fd);
		static	int			searchClient(std::string nick);
		static	bool		deleteClient(std::string nick);

		static	bool		insertChannel(channel &ch);
		static	channel&	searchChannel(std::string nameChannel);
		static	bool		srchChannel(std::string nameChannel);
		static	bool		joinClientChannel(std::string nameChannel, std::string nick);
		static	bool		deleteClientChannel(std::string nameChannel, std::string nick);
		static	bool		deleteChannel(std::string nick);

		static	bool		updateNickClient(std::string curr, std::string new_);
};

#endif