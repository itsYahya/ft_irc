#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "client.hpp"
# include <map>
# include <vector>

class channel
{
	public:
		typedef std::map<std::string, int>			clients_type;
		typedef clients_type::iterator				clients_iter_type;

	private:
		std::vector<std::string>					ban_clients;
		std::string									nameChannel;
		std::string									passwd;
		bool										isPasswd;
		std::string									topic;
		bool										isModerate;
		bool										isTopicProtected;
		size_t										limit;
		bool										noexternal;

	public:
		clients_type								clients;
		clients_iter_type							cls_iter;
		channel(std::string name);
		channel(std::string name, std::string passwd);
		~channel();
		std::string									getNameChannel() const;
		bool										insertClientToChannel(std::string name,int	fd);
		bool										searchClient(std::string nick);
		bool										deleteClient(std::string nick);
		void										setBannedClient(std::string host);
		void										deleteBannedClient(std::string host);
		std::string									getPasswd() const;
		bool										getIsPasswd() const;
		int											getBannedClient(std::string host);

		std::string									getInfo(std::string nick);
		static std::string							getInfosHeader(std::string nick);
		static std::string							getInfosFooter(std::string nick);

		clients_type								&getClients();
		std::string									geTopic() const;
		void										seTopic(const std::string &topic);

		void										moderate(const std::string &msg);
		bool										moderated();

		void										protecTopic(const std::string &msg);
		bool										topicProtected();
		bool										wantsMore();
		void										setLimit(size_t l, const std::string &msg);
		void										setKey(const std::string &key, const std::string &msg);
		void										setNoExternal();
		bool										noExteranl();

		void										notifi(const std::string &msg);
		size_t										empty();
};

#endif