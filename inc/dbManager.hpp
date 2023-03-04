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
		dbManager();

	public:
		typedef std::map<std::string, int>::iterator			iterator_clinet;
		typedef std::map<std::string, channel>::iterator		iterator_channel;
		typedef std::map<std::string, channel>					channels_type;
		typedef std::map<std::string, int>						clients_type;

		~dbManager();
		static	dbManager&			getInstance();
		
		static	bool				insertClient(std::string name, int fd);
		static	int					searchClient(std::string nick);
		static	bool				deleteClient(std::string nick);

		static	bool				insertChannel(channel &ch);
		static	iterator_channel	searchChannel(std::string nameChannel);
		static	bool				srchChannel(std::string nameChannel);
		static	bool				joinClientChannel(std::string nameChannel, std::string nick, int fd);
		static	bool				deleteClientChannel(std::string nameChannel, std::string nick);
		static	bool				deleteChannel(std::string nick);

		static	bool				updateNickClient(std::string curr, std::string new_);
		static	channels_type		&getChannels();
		static	clients_type		&getClients();

		static	bool				isEndChannelIter(iterator_channel iter);
		static  bool				isEndClientIter(iterator_clinet iter);
		void						getInfoBan(int fd, std::string nick, std::string nameChannel);
		void						getInfoInvalid(int fd, std::string nick);
		void						getInfoNewJoin(client &cl, std::string namechannel);
		void						getInfoListClInChannel(client &cl, std::string nameChannel, std::vector<client> &cls);
		void						getInfoPartChannel(client &cl, std::string namechannel, std::string body);
		void						getInfoPartError(client &cl, std::string namechannel, int num);
		void						sendMsgCls(std::string info, std::string nameChannel);
		std::string					processInfoCls(channel &ch, client &cl, std::vector<client> &cls);

};
#endif