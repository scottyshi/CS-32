#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
using namespace std;

int computeHash(int key)
{
	return key;
}

unsigned int computeHash(string key)
{//this hash function was taken from Carey's lecture
	unsigned int i, total = 0;

	for (i = 0; i < key.length(); i++)
	{
		total = total + (i + 1)*key[i];
	}
	return total;
}

unsigned int computeHash(unsigned short key)
{//they're both numbers anyways...
	unsigned int x = key;
	return x;
}

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	int cap;
	numbers.clear();
	if (s.length() / 2 + 512 > 16384)
		cap = 16384;
	else
		cap = s.length() / 2 + 512;
	//WHAT THOULS BE THE NUMBER OF BUCKETS BASED ON CAPACITY C??
	HashTable<string, unsigned short> table (2 * cap, cap);
	for (unsigned short i = 0; i < 256; i++)
	{
		char x = static_cast<char>(i);
		string st = "";
		st += x;
		table.set(st, i, true);
	}
	int nextFreeID = 256;
	string runSoFar = "";
	vector <unsigned short> V;//hold our compressed result
	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i];
		string expandedRun = runSoFar + c;
		unsigned short x;
		if (table.get(expandedRun, x))//returns true if got value
		{
			runSoFar = expandedRun;
			continue;
		}
		table.get(runSoFar, x);//gets the unsigned short runSoFar is mapped to in x
		V.push_back(x);
		table.touch(runSoFar);
		runSoFar = "";
		unsigned short cv = 0;
		string temp = "";
		temp += c;
		table.get(temp, cv);
		V.push_back(cv);
		if (!table.isFull())
		{
			table.set(expandedRun, nextFreeID);
			nextFreeID++;
		}
		else
		{
			unsigned short deleted;
			string k = "";
			table.discard(k, deleted); 
			table.set(expandedRun, deleted);
		}
	}
	if (runSoFar != "")
	{
		unsigned short x;
		table.get(runSoFar, x);
		V.push_back(x);
	}
	V.push_back(cap);

	numbers = V;
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	HashTable<unsigned short, string> table(2*(numbers[numbers.size() - 1]), numbers[numbers.size() - 1]);
	for (unsigned short i = 0; i < 256; i++)
	{
		char x = static_cast<char>(i);
		string st = "";
		st += x;
		table.set(i, st, true);
	}
	int nextFreeID = 256;
	string runSoFar = "";
	string output = "";//contains decompressed result
	for (int i = 0; i < numbers.size() -1; i++)
	{
		int us = numbers[i];
		if (us <= 255)
		{
			string stuff = "";
			table.get(numbers[i], stuff);
			output += stuff;
			if (runSoFar == "")
			{
				table.get(us, runSoFar);
				continue;
			}
			string expandedRun = runSoFar + stuff;
			if (table.set(nextFreeID, expandedRun))//returns false if full
				nextFreeID++;
			else
			{
				unsigned short deleted;
				string carey = "";
				table.discard(deleted, carey);  
				table.set(deleted, expandedRun);
			}
			runSoFar = "";
		}
		else//numbers[i] represents multicharacter string
		{
			string contents = "";
			if (!table.get(us, contents))//if it returned false
				return false;
			table.touch(us);
			output += contents;
			runSoFar = contents;
		}
	}
	s = output;
	return true;
}
