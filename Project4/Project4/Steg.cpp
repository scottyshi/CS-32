#include "provided.h"
#include <string>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn == "")
		return false;
	vector <string> lines;
	string parsed = hostIn;
	//parse everything in hostIn into lines, separated by \n or \r\n
	for (int i = 0; i < parsed.length(); i++)
	{
		int index = -1;
		//if i find a "\r\n" or a "\n" then I want everything up until that point
		if (parsed.find("\r\n") != string::npos)//it found a \r\n!
		{
			index = parsed.find("\r\n");//this holds the position of the "\"
			lines.push_back(parsed.substr(0, index));
			//takes everything up until the "\r\n"
			parsed = parsed.substr(index + 2, parsed.length() - 1);
		}
		else if (parsed.find("\n") != string::npos)//found a \n!
		{
			index = parsed.find("\n"); //this holds position of the "\"
			lines.push_back(parsed.substr(0, index));//takes in everything until the "\n"
			parsed = parsed.substr(index + 1, parsed.length() - 1);
		}//should only try to find one or the other in a given iteration
	}
	if (parsed != "")//picked up something at the end of the hostIn
		lines.push_back(parsed);

	for (int i = 0; i < lines.size(); i++)//getting rid of the excess space and tab
	{
		int last = lines[i].size() - 1;//accessing last value for each line
		while (last >= 0 && (lines[i][last] == ' ' || lines[i][last] == '\t'))
			last--;
		lines[i] = lines[i].substr(0, last + 1);
	}

	vector <unsigned short> codez;

	//compress the msg into vector of shorts
	Compressor::compress(msg, codez);
	string encoded = BinaryConverter::encode(codez);
	//encoded houses my 010101010101
	vector <string> bits;//these will house the 0101, based on how many lines there are
	int plusones = encoded.length() % lines.size();// (L/N) + 1
	int siz = encoded.length() / lines.size();
	for (int i = 0; i < plusones; i++)//adding the plus ones
	{
		bits.push_back(encoded.substr(0, siz + 1));
		encoded = encoded.substr(siz + 1);//cutting away parts of it
	}
	while (encoded != "")//getting the rest of the (L/N) 
	{
		bits.push_back(encoded.substr(0, siz));
		encoded = encoded.substr(siz);
	}

	for (int i = 0; i < lines.size(); i++)
		lines[i] += bits[i];//lines are now encoded

	//putting the encoded lines back into hostout
	for (int i = 0; i < lines.size(); i++)
		hostOut += lines[i] + '\n';//I want to load new lines to divide the string

	return true;  // This compiles, but may not be correct
}

bool Steg::reveal(const string& hostIn, string& msg)
{
	vector <string> lines;
	string parsed = hostIn;
	//parse everything in hostIn into lines, separated by \n or \r\n
	for (int i = 0; i < parsed.length(); i++)
	{
		int index = -1;
		//if i find a "\r\n" or a "\n" then I want everything up until that point
		if (parsed.find("\r\n") != string::npos)//it found a \r\n!
		{
			index = parsed.find("\r\n");//this holds the position of the "\"
			lines.push_back(parsed.substr(0, index));
			//takes everything up until the "\r\n"
			parsed = parsed.substr(index + 2, parsed.length() - 1);
		}
		else if (parsed.find("\n") != string::npos)//found a \n!
		{
			index = parsed.find("\n"); //this holds position of the "\"
			lines.push_back(parsed.substr(0, index));//takes in everything until the "\n"
			parsed = parsed.substr(index + 1, parsed.length() - 1);
		}//should only try to find one or the other in a given iteration
	}
	if (parsed != "")//picked up something at the end of the hostIn
		lines.push_back(parsed);

	//now lines vector holds the portions separated by the newline characters

	//now we need to parse the spaces and tabs away from the other stuff
	vector<string> bits; //this will hold the 0s and 1s for each of the items in lines
	for (int i = 0; i < lines.size(); i++)
	{
		int last = lines[i].size() - 1;
		while (last >= 0 && (lines[i][last] == ' ' || lines[i][last] == '\t'))
			last--;
		bits.push_back(lines[i].substr(last + 1));//we want to include index last+1
	}
	string encoded = "";
	for (int i = 0; i < bits.size(); i++)
		encoded += bits[i];//encoded has the 1s and 0s

	vector<unsigned short> nums;//this will house the decoded 1s and 0s

	if (!BinaryConverter::decode(encoded, nums))
		return false;
	Compressor::decompress(nums, msg);
	return true;
}