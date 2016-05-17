#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MenuItem
{
public:
	MenuItem(string nm) : m_name(nm) {}
	virtual ~MenuItem() {}
	string name() const { return m_name; }
	virtual bool add(MenuItem* m) = 0;
	virtual const vector<MenuItem*>* menuItems() const = 0;
private:
	string m_name;
};

class PlainMenuItem : public MenuItem   // PlainMenuItem allows no submenus
{
public:
	PlainMenuItem(string nm) : MenuItem(nm) {}
	virtual bool add(MenuItem* m) { return false; }
	virtual const vector<MenuItem*>* menuItems() const { return NULL; }
};

class CompoundMenuItem : public MenuItem  // CompoundMenuItem allows submenus
{
public:
	CompoundMenuItem(string nm) : MenuItem(nm) {}
	virtual ~CompoundMenuItem();
	virtual bool add(MenuItem* m) { m_menuItems.push_back(m); return true; }
	virtual const vector<MenuItem*>* menuItems() const { return &m_menuItems; }
private:
	vector<MenuItem*> m_menuItems;
};

CompoundMenuItem::~CompoundMenuItem()
{
	for (int k = 0; k < m_menuItems.size(); k++)
		delete m_menuItems[k];
}

void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m == nullptr)//base case, nothing to follow anymore
	{
		cout << endl;
		return;
	}
	string prt = path + m->name();
	if (prt.size() != 0)//not printing that empty line in beginning
		cout << prt << endl;
	if (m->menuItems() == NULL)//no submenus, just return
		return;
	for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
	{//iterate through submenus, don't add "/" if theres no name to be added to path
		if (path.size() == 0 && m->name().size() == 0)
			listAll(*it, path + m->name());
		else
			listAll(*it, path + m->name() + "/");
	}
}

void listAll(const MenuItem* m)  // one-parameter overload
{
	if (m != NULL)
		listAll(m, "");
}
