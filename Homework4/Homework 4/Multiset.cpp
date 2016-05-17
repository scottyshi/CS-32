#include "Multiset.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

void test()
{
	Multiset<int> mi;
	Multiset<string> ms;
	assert(mi.empty());
	assert(ms.size() == 0);
	assert(mi.uniqueSize() == 0);
	assert(mi.insert(10));
	assert(ms.insert("hello"));
	assert(mi.contains(10));
	assert(ms.count("hello") == 1);
	assert(mi.erase(10) == 1);
	string s;
	assert(ms.get(0, s) && s == "hello");
	Multiset<string> ms2(ms);
	ms.swap(ms2);
	ms = ms2;
	combine(mi, mi, mi);
	combine(ms, ms2, ms);
	assert(ms.eraseAll("hello") == 2);
	subtract(mi, mi, mi);
	subtract(ms, ms2, ms);
}
