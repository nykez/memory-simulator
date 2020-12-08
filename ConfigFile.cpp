#include <cstring>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <algorithm>

using namespace std;

//https://www.codespeedy.com/tokenizing-a-string-in-c/
vector<string> split(string& line, const char* delimiter)
{
	vector<string> v; //vector to hold tokens

	//buffer to hold input string contents
	char str[line.size() + 1];
	line.copy(str, line.size() + 1);
	str[line.size()] = '\0';
	
	//get the first token
	auto* token = strtok(str, delimiter);
	while (token != NULL)
	{
		v.emplace_back(token);
		//getting the next token
		//if there are no tokens left, NULL is returned
		token = strtok(NULL, delimiter);
	}
	
	return v;
}

vector<pair<string, string>> ReadConfigFile(string filename)
{
	// trim from start (in place)
	auto ltrim = [](std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}));
	};

	// trim from end (in place)
	auto rtrim = [](std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}).base(), s.end());
	};
	
	//Options used to read config file
	const auto BUFFER_LEN = 128;
	char buffer[BUFFER_LEN] = "";
	auto* line = buffer;
	unsigned long len = BUFFER_LEN;
	unsigned long read = 0;
	
	auto* fileaddress = fopen(filename.c_str(), "r"); //attempt to open the file to read
	if (fileaddress == nullptr) //if the file wasn't found
	{
		perror("Error opening config file");
		exit(EXIT_FAILURE);
	}

	vector<pair<string, string>> configOptions{}; //vector to hold the config options
	//If the file was found, read it line by line
	while ((read = getline(&line, &len, fileaddress)) != -1)
	{
		auto currentline = string{line};

		//If line contains ':', then split it
		if (currentline.find(':') != string::npos)
		{
			auto fields = split(currentline, ":");
			//Remove spaces and newline characters from each line
			for (auto &field : fields)
			{
				ltrim(field);
				rtrim(field);
			}

			pair<string, string> configPair{};
			configPair.first = fields[0];
			configPair.second = fields[1];
			configOptions.push_back(configPair);
		}
	}
	
	//Lambda function isInteger
	auto isInteger = [](const std::string & s) -> bool
	{
		if(s.empty() || !isdigit(s[0]) && (s[0] != '-') && (s[0] != '+'))
		{
			return false;
		}
		char * p;
		strtol(s.c_str(), &p, 10);
		return (*p == 0);
	};

	//Lambda function isYN
	auto isYN = [](const std::string & s) -> bool
	{
		if (s == "y"s || s == "n"s || s == "Y"s || s == "N"s)
		{
			return true;
		}
		return false;
	};

	//Validate the config options, checking that each is an integer or Y/N
	for (auto element : configOptions)
	{
		if (isInteger(element.second))
		{
			continue;
		}
		else if (isYN(element.second))
		{
			continue;
		}
		else
		{
			cerr << "Error reading value of key: "s + element.first << endl;
			throw("Malformed config file field");
		}
	}
	
	return configOptions;
}
