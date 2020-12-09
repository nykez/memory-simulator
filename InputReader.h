/*
 * David Nelson
 * OS Final Project
 * 12-09-2020
 * Operating Systems
 * InputReader.h
 * 
 */

#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstring>


/// <summary>
/// Handles reading of config and trace files.
/// </summary>
class InputReader
{
	FILE* trace_file{}; //pointer to the trace file
	bool trace_reached_eof = false; //bool signifying if EOF has been reached
	const static int trace_buffer_len = 64; //length of buffer to read lines in the trace file
	char trace_buffer[trace_buffer_len] = ""; //the buffer holding the current line in the trace file
	char* trace_line = trace_buffer; //a pointer to the buffer holding the current line in the trace file
	unsigned long trace_len = trace_buffer_len; //the length of the current line in the trace file
	unsigned long trace_read = 0; //the number of characters read
	int trace_line_counter = 0; //a counter to keep a current line count in the trace file
	
public:

	InputReader();

	static std::vector<std::string> split(std::string& line, const char* delimiter);
	std::map<std::string, std::string> ReadConfigFile(const std::string& filename) const;
	bool SetTraceFile(const std::string& filename);
	std::pair<bool, std::pair<std::string, std::string>> ReadTrace();

	bool is_integer(const std::string& s) const;
	bool is_hex(const std::string& s) const;
	static bool is_yn(const std::string& s);
	void ltrim(std::string& s) const;
	void rtrim(std::string& s) const;
	
	
};

/// <summary>
/// Default Constructor
/// </summary>
InputReader::InputReader() = default;

/// <summary>
/// Splits a string on a specified delimiter and returns the split string
/// as a vector of strings.
/// https://www.codespeedy.com/tokenizing-a-string-in-c/
/// </summary>
/// <param name="line">The string to split.</param>
/// <param name="delimiter">The delimiter to split on.</param>
/// <returns>A vector of strings from the parent string that was split.</returns>
inline std::vector<std::string> InputReader::split(std::string& line, const char* delimiter)
{
	std::vector<std::string> v; //vector to hold tokens

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

/// <summary>
/// Reads in a trace.config file and returns a map of config file options.
/// </summary>
/// <param name="filename">The path to the trace.config file.</param>
/// <returns>A pair indicating if the operation was successful and a config file options map.</returns>
inline std::map<std::string, std::string> InputReader::ReadConfigFile(const std::string& filename) const
{
	std::map<std::string, std::string> configOptions; //map to hold the config options
	
	//Options used to read config file
	const auto buffer_len = 512; //size for line buffer when reading config file
	char buffer[buffer_len] = ""; //buffer for lines when reading config file
	auto* line = buffer; //pointer to the buffer to hold each line
	unsigned long len = buffer_len; //the length of the buffer
	unsigned long read = 0; //the amount of characters read

	//Attept to open the input file
	auto* config_file = fopen(filename.c_str(), "r"); //attempt to open the file to read
	if (config_file == nullptr) //if the file wasn't found
	{
		perror("Error opening config file");
		exit(EXIT_FAILURE);
	}
	
	//If the file was found, read it line by line
	auto line_counter = 0; //counter for lines read
	while ((read = getline(&line, &len, config_file)) != -1)
	{
		auto current_line = std::string{line}; //treat current line as a string

		//If line contains ':', then split it
		if (current_line.find(':') != std::string::npos)
		{
			line_counter++;
			
			auto fields = split(current_line, ":");
			//Remove spaces and newline characters from each line
			for (auto &field : fields)
			{
				ltrim(field);
				rtrim(field);
			}

			//Check if the value is an integer or "Y/y" or "N/n"
			if (is_integer(fields[1]) == true || is_yn(fields[1]) == true)
			{
				configOptions.insert({fields[0], fields[1]});
			}
			else
			{
				std::cerr << "Error reading value at line: " << line_counter << std::endl;
				throw("Malformed config file field");
			}
		}
		else
		{
			line_counter++;
		}
	}
	
	return configOptions;
}

/// <summary>
/// Sets the location of the trace file.
/// </summary>
/// <param name="filename">The trace file name.</param>
/// <returns>True if trace file was found; False if there was
/// an error finding the trace file.</returns>
inline bool InputReader::SetTraceFile(const std::string& filename)
{
	trace_file = fopen(filename.c_str(), "r");
	if (trace_file == nullptr)
	{
		perror("Error opening trace file");
		exit(EXIT_FAILURE);
	}

	return true;
}

/// <summary>
/// Returns a trace from the trace file and signifies if the end of
/// the file was reached.
/// </summary>
/// <returns>A pair of format <bool:pair<string:string>> that signifies if EOF was reached
/// and a pair containing trace properties.</returns>
inline std::pair<bool, std::pair<std::string, std::string>> InputReader::ReadTrace()
{
	std::string trace_access; //holds the current trace read/write access type
	std::string trace_hex_address; //holds the current trace hex-address

	//Check if the end of the trace file was reached
	if ((trace_read = getline(&trace_line, &trace_len, trace_file)) != -1)
	{
		//if not at end of the trace file
		trace_line_counter++; //increment the line counter
		
		auto current_line = std::string{trace_line}; //treat the current line as a string

		//If trace has a ':' delimiter
		if (current_line.find(':') != std::string::npos)
		{
			//split line into Access and Address
			auto fields = split(current_line, ":");
			for (auto &field : fields)
			{
				//trim the elements on left and right
				ltrim(field);
				rtrim(field);
			}

			//Check if first element is valid access type
			if (fields[0] == "R" || fields[0] == "W" || fields[0] == "r" || fields[0] == "w")
			{
				//Read/Write Field is valid
				trace_access = fields[0];
			}
			else
			{
				//if the access type field isn't valid
				std::cerr << "Could not read access type parameter at line: " << trace_line_counter << std::endl;
				exit(EXIT_FAILURE);
			}
			
			//Check if second element is valid hex address
			if (is_hex(fields[1]) == true)
			{
				//Hex address field is valid
				trace_hex_address = fields[1];
			}
			else
			{
				//if the hex-address field isn't valid
				std::cerr << "Could not read hex-address parameter at line: " << trace_line_counter << std::endl;
				exit(EXIT_FAILURE);
			}

			//return the valid trace
			return std::make_pair(false, std::make_pair(trace_access, trace_hex_address));
		}
		else
		{
			//if the trace doesn't have a ':' delimiter
			std::cerr << "Error reading trace at line: " << trace_line_counter << std::endl;
			exit(EXIT_FAILURE);
		}	
	}
	else
	{
		//if EOF was reached
		return std::make_pair(true, std::make_pair(trace_access, trace_hex_address));
	}
	
	return {};
}

//Lambda function determines if string is an integer
// https://stackoverflow.com/a/2845275/14352318
bool inline InputReader::is_integer(const std::string& s) const
{
	if(s.empty() || !isdigit(s[0]) && (s[0] != '-') && (s[0] != '+'))
	{
		return false;
	}
	
	char * p;
	strtol(s.c_str(), &p, 10);
	return (*p == 0);
}

/// <summary>
/// Checks if the specified string is a hexadecimal number.
/// </summary>
/// <param name="s">The string to check.</param>
/// <returns>True if string is a hexadecimal value;
/// False if string isn't a hexadecimal value</returns>
bool inline InputReader::is_hex(const std::string& s) const
{
	//Check if string is a hexadecimal value
	if (s.empty() || !isxdigit(s[0]))
	{
		return false;
	}

	char * p;
	strtol(s.c_str(), &p, 16);
	return (*p == 0);
}

/// <summary>
/// Determines if string is 'Y/y' or 'N/n'
/// </summary>
/// <param name="s">The string to check.</param>
/// <returns>True if string is "Y/y" or "N/n"; False if string is anything else.</returns>
bool inline InputReader::is_yn(const std::string& s)
{
	if (s == "y" || s == "n" || s == "Y" || s == "N")
	{
		return true;
	}
	
	return false;
}

//Trim string from start (in place)
// https://stackoverflow.com/a/217605/14352318
void InputReader::ltrim(std::string &s) const
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

//Trim string from end (in place)
//https://stackoverflow.com/a/217605/14352318
void InputReader::rtrim(std::string &s) const
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

#endif // INPUTREADER_H
