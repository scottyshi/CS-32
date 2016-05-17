#include <iostream>
#include <string>
using namespace std;

//Your declarations and implementations would go here
class Character//pure virtual class character
{
public:
	Character(string name) : m_name(name){}
	virtual string name() const { return m_name; }
	virtual void printWeapon() const = 0;
	virtual void attackAction() const = 0;
	virtual ~Character(){}
private:
	string m_name;
};

class Dwarf: public Character
{
public:
	Dwarf(string name):Character(name){}
	virtual void printWeapon() const
	{
		cout << "an axe";
	}
	virtual void attackAction() const
	{
		cout << "rushes toward the enemy";
	}
	virtual ~Dwarf()
	{
		cout << "Destroying " << this->name() << " the dwarf" << endl;
	}
};

class Elf : public Dwarf
{
public:
	Elf(string name, int arrows) :Dwarf(name), m_arrows(arrows){}
	virtual void printWeapon() const
	{
		cout << "a bow and quiver of " << m_arrows << " arrows";
	}
	virtual ~Elf()
	{
		cout << "Destroying " << this->name() << " the elf" << endl;
	}
private:
	int m_arrows;
};

class Boggie : public Character
{
public:
	Boggie(string name) :Character(name){}
	virtual void printWeapon() const
	{
		cout << "a short sword";
	}
	virtual void attackAction() const
	{
		cout << "whimpers";
	}
	virtual ~Boggie()
	{
		cout << "Destroying " << this->name() << " the boggie" << endl;
	}
};

