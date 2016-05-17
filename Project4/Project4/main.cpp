#include "HashTable.h"
#include "provided.h"
#include "http.h"
#include <iostream>
using namespace std;


int main()
{
	
	string msg = "This class is finally over!";
	string host;
	HTTP().set("http://a.com", "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ");
	if(WebSteg::hideMessageInPage("http://a.com", msg, host) )
	{
	HTTP().set("http://a.com",host); // replace the original webpage by the new content
	string msg2;
	if(WebSteg::revealMessageInPage("http://a.com", msg2)) {
	if(msg == msg2)
	cout << "Successfully hide and reveal message in pages!!\n" << msg << endl;
	else {
	cout << "Hidden message and revealed message do not match!!" << endl;
	cout << "Hidden message:   " << msg << endl;
	cout << "Revealed message: " << msg2 << endl;
	}
	}
	else
	cout << "Failed to reveal message in page!!" << endl;

	}
	else
	cout << "Failed to hide message in page!!" << endl;
	
}