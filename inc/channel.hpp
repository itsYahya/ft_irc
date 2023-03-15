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

		std::string									getPasswd() const;
		bool										getIsPasswd() const;
		bool										getBannedClient(std::string host);

		std::string									getInfo(std::string nick);
		static std::string							getInfosHeader(std::string nick);
		static std::string							getInfosFooter(std::string nick);

		clients_type								&getClients();
		std::string									geTopic() const;
		void										seTopic(const std::string &topic);
};

#endif