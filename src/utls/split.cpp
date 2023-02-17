#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string>	split(std::string str, char sep){
	std::stringstream			stream(str);
	std::string					string;
	std::vector<std::string>	vector;

	while (std::getline(stream, string, sep)){
		if (!string.empty())
			vector.push_back(string);
	}
	return (vector);
}
