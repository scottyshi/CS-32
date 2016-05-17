#include "Multiset.h"
#include <cassert>
#include <iostream>

using namespace std;

Multiset::Multiset()
{
	head = nullptr; //this indicates an empty linked list
	tail = nullptr;
}

bool Multiset::empty() const //since items is a pointer to first element of array
{
	return (head == nullptr); //this is an empty list
}

int Multiset::size() const//assuming my insert function already takes care of duplicates
{
	int cnt = 0;
	CareyNode *ptr = head;
	while (ptr != nullptr)
	{
		cnt += ptr->count;
		ptr = ptr->next;
	}
	return cnt;
}

int Multiset::uniqueSize() const//same thing as size, but i don't iterate based on CareyNode's count
{
	int cnt = 0;
	CareyNode *ptr = head;
	while (ptr != nullptr)
	{
		cnt++;
		ptr = ptr->next;
	}
	return cnt;
}

bool Multiset::insert(const ItemType& value)
{
	if (head == nullptr)//case 1 -- empty list
	{
		head = new CareyNode();
		head->m_val = value;
		head->count = 1;
		head->next = nullptr;
		head->prev = nullptr;
		tail = head;
		return true; //exits
	}
	CareyNode *ptr = head;
	if (ptr->m_val == value)
	{
		ptr->count++;
		return true;
	}
	while (ptr->next != nullptr)//case 2 -- item already exists
	{
		if (ptr->m_val == value)
		{
			ptr->count++;
			return true;
		}
		ptr = ptr->next;
	}
	if (ptr->m_val == value)
	{
		ptr->count++;
		return true;
	}
	//Case 3 -- item doesn't exist, add it to end of list
	//ptr points to the last Node in the linked list right now
	tail = new CareyNode(); //create new element
	tail->m_val = value;
	tail->count = 1;
	tail->prev = ptr;
	tail->next = nullptr;
	ptr->next = tail;
	return true;
}

int Multiset::erase(const ItemType& value)
{
	if (empty()) //can't erase nothing
		return 0;
	CareyNode *ptr = head;
	while (ptr != nullptr)//case 2, it's in the list
	{
		if (ptr->m_val == value)
		{
			if (ptr->count > 1)
			{
				ptr->count--;
				return 1;
			}
			else
			{
				eraseAll(value);
				return 1;
			}
		}
		ptr = ptr->next;
	}
	//case 3 it's not on the list
	return 0;
}

int Multiset::eraseAll(const ItemType& value)
{
	if (empty())//case 1, empty list
		return 0;
	CareyNode *ptr = this->head;
	if (this->head == tail)//case 1.3333 -- only item in the list
	{
		int deleted = ptr->count;
		delete ptr;
		this->head = nullptr;
		tail = nullptr;
		return deleted;
	}
	if (ptr == head && ptr->m_val == value)
	{
		int deleted = ptr->count;
		head = ptr->next;
		head->prev = nullptr;
		delete ptr;
		return deleted;
	}
	while (ptr->next != nullptr)//case 2, it's somewhere in the list
	{
		if (ptr->m_val == value)//it's somewhere in between head and tail
		{
			int deleted = ptr->count;
			CareyNode *previous = ptr->prev;
			CareyNode *nextone = ptr->next;
			previous->next = nextone;
			nextone->prev = previous;
			delete ptr;
			return deleted;
		}
		ptr = ptr->next;
	}
	//case 3, it's at the end of the list and we have to update tail
	if (ptr->m_val == value)
	{
		int deleted = ptr->count;
		tail = ptr->prev;
		tail->next = nullptr;
		delete ptr;
		return deleted;
	}
	return 0;
}

bool Multiset::contains(const ItemType& value) const
{
	CareyNode * ptr = head; //very simple traversal
	while (ptr != nullptr)
	{
		if (ptr->m_val == value)
			return true;
		ptr = ptr->next;
	}
	return false;
}

int Multiset::count(const ItemType& value) const
{
	CareyNode * ptr = head; //same kind of traversal as previous function
	while (ptr != nullptr)
	{
		if (ptr->m_val == value)
			return ptr->count;
		ptr = ptr->next;
	}
	return 0;
}

int Multiset::get(int i, ItemType& value) const
{	
	if (i < 0 || i >= uniqueSize())
		return 0;
	int iterator;
	CareyNode * ptr = head;
	for (iterator = 0; iterator != i; iterator++)
		ptr = ptr->next;
	value = ptr->m_val;
	return ptr->count;
}

void Multiset::swap(Multiset& other) //swapping the heads and tails...ez
{
	CareyNode *temp = other.head;
	other.head = this->head;
	this->head = temp;
	CareyNode *haha = other.tail;
	other.tail = this->tail;
	this->tail = haha;
}

Multiset::~Multiset()
{
	if (empty())
		return;
	while (head != tail)//there is more than one item in the list
	{
		CareyNode *ptr = tail; //temp to delete tail
		tail = tail->prev; 
		delete ptr;
	}//now there is only one item in the list
	delete head;
	head = nullptr;
	tail = nullptr;
}

Multiset::Multiset(const Multiset& src)
{
	if (src.empty())
		return;
	head = new CareyNode(); //create a single item in the list
	head->prev = nullptr;
	head->next = nullptr;
	head->m_val = src.head->m_val;
	head->count = src.head->count;
	CareyNode *ptr = src.head->next;//already added the head
	while (ptr != nullptr)//traversing through source
	{
		for (int i = 0; i < ptr->count; i++)//I want to use my insert functions "count" times
			insert(ptr->m_val);
		ptr = ptr->next;
	}
	//now that my Multiset has been copied over, I want to update tail
	CareyNode *ptragain = head;
	while (ptragain->next != nullptr)
	{
		ptragain = ptragain->next;
	}
	tail = ptragain;
}

Multiset& Multiset::operator= (const Multiset& src)
{
	if (this == &src)//aliasing prevention
		return *this;
	//TAKEN FROM THE DESTRUCTOR
	while (head != tail)//there is more than one item in the list
	{
		CareyNode *ptr = tail; //temp to delete tail
		tail = tail->prev;
		delete ptr;
	}//now there is only one item in the list
	delete head;
	head = nullptr;
	tail = nullptr;

	if (src.empty()) //TAKEN FROM COPY CONSTRUCTOR
		return *this;//since this is already deleted
	head = new CareyNode(); //create a single item in the list
	head->prev = nullptr;
	head->next = nullptr;
	head->m_val = src.head->m_val;
	head->count = src.head->count;
	CareyNode *ptr = src.head->next;//already added the head
	while (ptr != nullptr)
	{
		for (int i = 0; i < ptr->count; i++)//I want to use my insert functions "count" times
			insert(ptr->m_val);
		ptr = ptr->next;
	}
	//now that my Multiset has been copied over, I want to update tail
	CareyNode *ptragain = head;
	while (ptragain->next != nullptr)
	{
		ptragain = ptragain->next;
	}
	tail = ptragain;//done copying

	return *this;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	//CHECK FOR ALIASING!!!!!!!!!!!!!!!!
	if (&result == &ms1 && &result == &ms2)//if all the same, add results items another time
	{
		for (int i = 0; i < result.uniqueSize(); i++)
		{
			ItemType heh;
			int counter = result.get(i, heh);
			for (int j = 0; j < counter; j++)
				result.insert(heh);
		}
		return;
	}

	//if (&result == &ms1)//already handled in copy constructor-- alias prevention
	if (&result == &ms2)//simply insert ms1 into result which already has ms2
	{
		for (int i = 0; i < ms1.uniqueSize(); i++)
		{
			ItemType haha;
			int counter = ms1.get(i, haha);
			for (int j = 0; j < counter; j++)//avoid infinite for loop by using a variable
				result.insert(haha);
		}
		return;//sucessfully added
	}
		
	//I will attempt to insert all of ms2's items into ms1
	//storing it into result simply uses the assignment operator
	result = ms1; // first put all of ms1 into result
	for (int i = 0; i < ms2.uniqueSize(); i++)//find how much ms2 contains
	{
		ItemType hehehe;//this will be used in the get function in the next line
		int counter = ms2.get(i, hehehe);
		for (int j = 0; j < counter; j++)//this will help insert hehehe the "count" number of times
			result.insert(hehehe);
	}
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{//same thing as combine, but we will be calling erase All instead of insert
	if (&ms1 == &ms2)//case 0 result should be empty // even if result is aliased with ms1 and ms2
	{
		Multiset x;//i'll utilize my constructor and assignment operator
		result = x;//it will just be an empty multiset
		return;//result is an empty list
	}
	//case 1 --result is aliased with ms2 (then I want to combine ms1 and ms2 and then subtract 2x ms2
	if (&ms2 == &result)
	{
		Multiset temp = ms2;//using copy constructor to make a copy of ms2
		result = ms1;//ms2 is gone now, result is now a copy of ms1
		for (int i = 0; i < temp.uniqueSize(); i++)
		{
			ItemType killme;
			int counter = temp.get(i, killme);
			for (int j = 0; j < counter; j++)//subtracts all the killme from ms2
				result.erase(killme);
		}
	}

	//case 2 -- result is aliased with ms1
	//case 3 -- result is not aliased with anything
	//either way, I want to set result to ms1 (my copy constructor protects aliasing) and subtract ms2 from it
	result = ms1;
	for (int i = 0; i < ms2.uniqueSize(); i++)
	{
		ItemType killme;
		int counter = ms2.get(i, killme);
		for (int j = 0; j < counter; j++)//subtracts all the killme from ms2
			result.erase(killme);
	}
}