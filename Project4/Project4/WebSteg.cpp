#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	string bits = "";
	if (!HTTP().get(url, bits))
		return false;
	Steg::hide(bits, msg, host);
	return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	string bits = "";
	if (!HTTP().get(url, bits))
		return false;
	Steg::reveal(bits, msg);
	return true;
}
