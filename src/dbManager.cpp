#include "dbManager.hpp"

std::map<std::string, client> dbClient::clients;

dbClient::dbClient(){}
dbClient::~dbClient() {}


dbClient*	dbClient::getInstance()
{
	if (instance == 0)
		instance = new dbClient();
	return (instance);
}