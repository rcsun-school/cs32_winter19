#ifndef SET_H
#define SET_H
#include <string>
using ItemType = std::string;
class Set {
public:
	Set();
	Set(const Set& other);
	~Set();
	Set& operator=(const Set& other);
	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	void swap(Set& other);


private:
	bool m_empty;
	int m_size;
	struct Node {
		Node * previous;
		Node * next;
		ItemType value;
	};
	Node * m_head;
	Node * m_tail;
};

void unite(const Set& s1, const Set& s2, Set& result);

void subtract(const Set& s1, const Set& s2, Set& result);


#endif
