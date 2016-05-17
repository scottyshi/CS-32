#ifndef HASHTABLE_H
#define HASHTABLE_H

template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity)
	{
		m_cap = capacity;
		m_numBuck = numBuckets;
		m_oldest = nullptr;
		occupied = 0;
		m_newest = nullptr;
		hash = new Node*[numBuckets];
		for (int i = 0; i < numBuckets; i++)
			hash[i] = nullptr;
	}
	~HashTable()
	{
		for (int i = 0; i < m_numBuck; i++)
		{
			if (hash[i] != nullptr)
			{
				Node * killme = hash[i];
				if (killme->next == nullptr)
				{
					delete killme;
					continue;
				}
				Node* temp = killme->next;
				while (temp->next != nullptr)
				{
					delete killme;
					killme = temp;
					temp = killme->next;
				}
			}
		}
		delete[] hash;
	}
	bool isFull()	const
	{
		return (occupied == m_cap);
	}
	//update occupied member variable in these methods!
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false)
	{
		if (isFull())
			return false;
		Node* add = new Node();
		add->m_val = value;
		add->next = nullptr;
		add->newer = nullptr;
		add->older = nullptr;
		add->perm = permanent;
		add->key = key;

		bool added = false;
		if (m_oldest == nullptr && !permanent)//if theres nothing originally in the newer/older linked list
		{
			m_oldest = add;
			m_newest = add;
			added = true;
		}

		unsigned int computeHash(KeyType x);//inlined hash function
		unsigned int bucket = computeHash(key) % m_numBuck;
		Node* temp = hash[bucket];
		if (hash[bucket] == nullptr)//if theres nothing originally in the bucket
		{
			if (!added && !permanent)
			{
				m_newest->newer = add;
				add->older = m_newest;
				m_newest = add;
				m_newest->newer = nullptr;
			}
			hash[bucket] = add;
			occupied++;
			return true;
		}
		while (temp != nullptr)//i want to associate this key with another value
		{
			if (key == temp->key)//if value already exists
			{
				if (temp->perm)
					return false;//i return false because nothing was changed

				temp->m_val = add->m_val;//updated value
				//the rest is updating newer/older link list
				if (temp->older != nullptr && temp->newer != nullptr)//case 1, it's somewhere in between the new list
				{
					temp->older->newer = temp->newer;//linking older and newer
					temp->newer->older = temp->older;
				}
				else if (temp == m_oldest)//it's the oldest item
				{
					if (m_oldest == m_newest)//it's the only item in the list
						return true;//nothing else to be done here
					m_oldest->newer->older = nullptr;
				}
				else//temp is the newest item already
					return true;

				m_newest->newer = temp;//updating newest 
				temp->older = m_newest;
				temp->newer = nullptr;
				m_newest = temp;
				return true;
			}
			if (temp->next == nullptr)//so i can modify temp
				break;
			temp = temp->next;//CODE WILL ONLY GET SO FAR
		}		//IF A DIFFERENT KEY HASHED TO THIS BUCKET
		temp->next = add;//adding the new association into the hash table
		occupied++;//inc counter of items inside hash table

		if (!permanent)//if new value is not permanent, then i must
		{				//update my newer/older linked list
			if (m_oldest == nullptr)//nothing in the newly add ll!
			{
				m_oldest = m_newest = add;//now there is a new item
				return true;
			}
			if (m_oldest == m_newest)//there's only one in newly added ll
			{
				add->older = m_oldest;
				add->newer = nullptr;
				m_newest = add;
				m_oldest->newer = m_newest;
				return true;//now there are two
			}
			//if there are 2 or more items in the list, simply update m_newest
			m_newest->newer = add;
			add->older = m_newest;
			add->newer = nullptr;
			m_newest = add;
		}
		return true;
	}

	bool get(const KeyType& key, ValueType& value)	const
	{
		unsigned int computeHash(KeyType x);//inlined hash function
		unsigned int bucket = computeHash(key) % m_numBuck;
		Node* temp = hash[bucket];
		while (temp != nullptr)
		{
			if (temp->key == key)
			{
				value = temp->m_val;
				return true;
			}
			temp = temp->next;
		}
		return false;
	}

	bool touch(const KeyType& key)
	{
		unsigned int computeHash(KeyType x);//inlined hash function
		unsigned int bucket = computeHash(key) % m_numBuck;
		Node* temp = hash[bucket];
		
		while (temp != nullptr)
		{
			if (key == temp->key)//finding the matching key
			{
				if (temp->perm)
					return false;//cannot update this in the new ll
				//the rest is updating newer/older link list
				if (temp->older != nullptr && temp->newer != nullptr)//case 1, it's somewhere in between the new list
				{
					temp->older->newer = temp->newer;//linking older and newer
					temp->newer->older = temp->older;
				}
				else if (temp == m_oldest)//it's the oldest item
				{
					if (m_oldest == m_newest)//it's the only item in the list
						return true;//nothing else to be done here
					m_oldest->newer->older = nullptr;
					m_oldest = m_oldest->newer;
				}
				else//temp is the newest item already
					return true;

				m_newest->newer = temp;//updating newest 
				temp->older = m_newest;
				temp->newer = nullptr;
				m_newest = temp;
				return true;
			}
			temp = temp->next;
		}
		return false;
	}

	bool discard(KeyType& key, ValueType& value)
	{
		if (m_oldest == nullptr)//no non-permanent associations in table.
			return false;
		key = m_oldest->key;//retrieving the oldest key and value
		value = m_oldest->m_val;
		unsigned int computeHash(KeyType x);//inlined hash function
		unsigned int bucket = computeHash(key) % m_numBuck;
		hash[bucket] = nullptr;//no longer has an association
		
		if (m_newest == m_oldest)//only one item
		{
			delete m_newest;

			m_newest = nullptr;
			m_oldest = nullptr;
			occupied--;
			return true; //succesfully deleted the only item in "recently created"
		}
		//otherwise, if there isn't only one item
		Node* nextoldest = m_oldest->newer;//update the new oldest item
		delete m_oldest;
		occupied--;
		m_oldest = nextoldest;
		m_oldest->older = nullptr;

		return true;//successfully deleted oldest
	}

private:
	struct Node
	{
		bool perm;
		KeyType key;
		ValueType m_val;
		Node* newer;
		Node* older;
		Node* next;
	};
	int m_cap;
	int m_numBuck;
	int occupied;
	Node** hash;//pointer to your hash table which is an array of pointers
	Node* m_newest;//like the tail of the newer/older link list
	Node * m_oldest;//like the head of the newer/older link list
	
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
};

#endif