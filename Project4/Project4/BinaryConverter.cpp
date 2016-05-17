#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	string encoded = "";
	for (int i = 0; i < numbers.size(); i++)
		encoded += convertNumberToBitString(numbers[i]);
	string spacetab = "";
	for (int i = 0; i < encoded.size(); i++)
	{
		if (encoded[i] == '0')
			spacetab += ' ';
		else
			spacetab += '\t';
	}
	return spacetab; 
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{//bitstring should only be tabs and spaces
	if (bitString.size() % 16 != 0)
		return false;
	numbers.clear();//will set numbers back to empty vector
	string onesnzero = "";
	unsigned short num = 0;
	for (int i = 0; i < bitString.size(); i++)
	{
		if (bitString[i] == '\t')
			onesnzero += "1";
		else if (bitString[i] == ' ')
			onesnzero += "0";
		else 
			return false;
		if (i % 16 == 15) //i have reached 16 1s and 0s inside my onesnzero string
		{
			if (!convertBitStringToNumber(onesnzero, num))//added security?
				return false;
			numbers.push_back(num);
			onesnzero = "";//resetting to empty string;
		}
	}
	return true;  // This compiles, but may not be correct
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
