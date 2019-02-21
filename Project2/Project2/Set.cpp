#include "Set.h"

Set::Set() : m_empty(true), m_size(0), m_head(nullptr), m_tail(nullptr) { //default constructor
																			//construct an empty Set
}

//copy constructor
//constructs a new Set with the same data members as argument
Set::Set(const Set& other) {  
	m_empty = other.m_empty;
	m_size = other.m_size;
	m_head = nullptr;
	if (!other.empty()) {
		m_head = new Node;
		Node * p1 = m_head;
		for (Node * p2 = other.m_head; p2 != nullptr; p2 = p2->next) { //
			p1->value = p2->value;
			if (p2->next != nullptr) {
				p1->next = new Node;
				p1 = p1->next;
			}
		}
		m_tail = p1;
		p1->next = nullptr;
	}
}

//destructor 
//delete every node in Set
Set::~Set() {
	Node * p = m_head;
	while (p != nullptr) {
		Node * killme = p;
		p = p->next;
		delete killme;
	}
}

//assignment operator
Set& Set::operator=(const Set& other) {
	if (this == &other) {
		return *this;
	}
	//clear out existing values in receiving set
	Node * p = m_head;
	while (p != nullptr) {
		Node * killme = p;
		p = p->next;
		delete killme;
	}
	m_size = 0;
	m_empty = other.m_empty;
	m_head = nullptr;
	//if other Set is only one Node
	//simply allocate the head Node
	if (other.size() == 1) {
		m_head = new Node;
		m_head->value = other.m_head->value;
		m_tail = m_head;
		m_size++;
		return *this;
	}
	//traverse other Node while allocating Nodes in the constructed Set
	if (!other.empty()) {
		m_head = new Node;
		p = m_head;
		m_size = other.size();
		for (Node * p2 = other.m_head; p2 != nullptr; p2 = p2->next) {
			if (p2->next != nullptr) {
				p->value = p2->value;
				p->next = new Node;
				p = p->next;
			}
			else {
				p->value = p2->value;
				m_tail = p;
				p->next = nullptr;
			}
		}
		
	}
	return *this;
	
}

bool Set::empty() const{
	return m_empty;
}

int Set::size() const {
	return m_size;
}

//Set::insert()
//returns false if Set already contains passed in value
//returns true if value is not found in Set, and allocates a Node to hold value in Set
//Set holds values in increasing order left to right
bool Set::insert(const ItemType& value) {
	if (this->contains(value)) {
		return false;
	}
	//if currently empty
	//allocate a new head Node and assign value
	if (m_empty) {
		m_head = new Node;
		m_head->value = value;
		m_head->next = nullptr;
		m_size++;
		m_empty = false;
		m_tail = m_head;
		return true;
	}
			Node * p = new Node;
			p->value = value;
	//if the new node belongs after the head node
	//and the head is the only node
		
	if (m_size == 1 && m_head->value < value) {
		m_head->next = p;
		p->previous = m_head;
		p->next = nullptr;
		m_tail = p;
		m_size++;
		return true;
	}
		

	//if p belongs in front of the Set, reposition head node
	else if (m_head->value > value) {
		p->next = m_head;
		m_head->previous = p;
		m_head = p;
		m_size++;
		return true;
	}
	else {
		//traverse the Set for the Node's rightful spot
		for (Node * ptr = m_head; ptr->next != nullptr; ptr = ptr->next) {
			if (ptr->next->value > value) {
				p->previous = ptr;
				p->next = ptr->next;
				p->next->previous = p;
				ptr->next = p;
				m_size++;
				return true;
			}
		}
		//or stick the new Node at the end
		m_tail->next = p;
		p->previous = m_tail;
		p->next = nullptr;
		m_tail = p;
		m_size++;
		return true;
	}

}


//bool Set::erase() 
//returns false if value is not found in Set
//returns true if value is found in Set and deletes the corresponding Node
bool Set::erase(const ItemType& value) {
	if (!contains(value)) {
		return false;
	}
	//if Set size is 1, set head pointer to nullptr
	//and delete node
	else if (m_size == 1) { 
		Node * killme = m_head;
		m_head = nullptr;
		delete killme;
		m_tail = m_head;
		m_empty = true;
		m_size--;
		return true;
	}
	//if the head node is the target
	//reassign head pointer to the second node in Set
	//delete target node
	else if (m_head->value == value) {
		Node * killme = m_head;
		m_head = m_head->next;
		delete killme;
		m_size--;
		return true;
	}
	//traverse set until we find value
	else {
		for (Node * ptr = m_head; ptr->next != nullptr; ptr = ptr->next) {
			if (ptr->next->value == value) {
				if (m_tail == ptr) { //update tail if it is target
					m_tail = m_tail->previous;
				}
				Node * killme = ptr->next;
				ptr->next = killme->next;
				killme->next->previous = ptr;
				delete killme;
				m_size--;
				return true;
			}
		}
		return false; //if for some reason node was not found up to here, return false 
	}
}

//bool Set::contains()
//returns true if argument value is found in Set
//returns false if value is not found in Set
bool Set::contains(const ItemType& value) const {
	if (m_empty) {
		return false;
	}
	for (Node * ptr = m_head; ptr != nullptr; ptr = ptr->next) { 
		if (ptr->value == value) {
			return true;
		}
	}
	return false;
}

//void Set::get()
//inserts the value in the Set greater than pos values into argument pos
//returns true upon successful completion
//returns false if set is empty, pos is greater than or equal to Set size,
// or if pos is less than 0
bool Set::get(int pos, ItemType& value) const {
	if (m_empty || pos >= m_size || pos < 0) {
		return false;
	}
	Node * ptr = m_head;
	for (int i = 0; i < pos; i++) {
		ptr = ptr->next;
	}
	value = ptr->value;
	return true;
}

//void Set::swap()
//exchanges data between two Sets so that one Set receives the other's data and vice versa
void Set::swap(Set& other) {
	Node * temp = other.m_head;
	bool temp_empty = other.m_empty;
	int temp_size = other.size();
	Node * temp2 = other.m_tail;
	other.m_head = m_head;
	other.m_size = m_size;
	other.m_empty = m_empty;
	other.m_tail = m_tail;
	m_head = temp;
	m_size = temp_size;
	m_empty = temp_empty;
	m_tail = temp2;

}

//void unite()
//values found in either or both s1 or/and s2 are added to result

void unite(const Set& s1, const Set& s2, Set& result) {
	//if result is either s1 or s2
	//insert values from the other set into result
	if (&result == &s1) {
		int size = s2.size();
		for (int i = 0; i < size; i++) {
			ItemType s;
			s2.get(i, s);
			result.insert(s);
		}

	}
	else if (&result == &s2) {
		int size = s1.size();
		for (int i = 0; i < size; i++) {
			ItemType s;
			s1.get(i, s);
			result.insert(s);
		}
	}
	else { //result is not a coreference to s1 or s2
		//clean out result if needed
		if (!result.empty()) {
			int size = result.size();
			for (int i = 0; i < size; i++) {
				ItemType s;
				result.get(0, s);
				result.erase(s);
			}
		}
		//result is empty, add values from s1 into result
		int size = s1.size();
		for (int i = 0; i < size; i++) {
			ItemType s;
			s1.get(i, s);
			result.insert(s);
		}
		//now add values from s2 into result
		size = s2.size();
		for (int k = 0; k < size; k++) {
			ItemType s;
			s2.get(k, s);
			result.insert(s);
		}
	}

}

//void subtract()
//add values from s1 that are not found in s2 into result

void subtract(const Set& s1, const Set& s2, Set& result) {
	//if result is the same reference as s1, traverse s2 and erase 
	//matching values
	if (&result == &s1) {
		for (int i = 0; i < result.size(); i++) {
			ItemType s;
			result.get(i, s);
			if (s2.contains(s)) {
				result.erase(s);
				i--;
			}
		}
		return;
	}
	//if result is the same reference as s2, create a temporary 
	//set to save s2 values before emptying result, then dispose of the temporary set
	if (&result == &s2) {
		Set temp(s2); //store s2 values into temp
		int size = result.size();
		//empty result
		for (int i = 0; i < size; i++) {
			ItemType s;
			result.get(0, s);
			result.erase(s);
		}
		size = s1.size();
		//fill result with s1 values
		for (int j = 0; j < size; j++) {
			ItemType t;
			s1.get(j, t);
			if (!temp.contains(t)) {
				result.insert(t);
			}
		}
		size = temp.size();
		//empty temp to avoid memory leak
		for (int k = 0; k < size; k++) {
			ItemType u;
			temp.get(0, u);
			temp.erase(u);
		}
		return;
	}
	//clean out result if needed
	if (!result.empty()) {
		int size = result.size();
		for (int i = 0; i < size; i++) {
			ItemType s;
			result.get(0, s);
			result.erase(s);
		}
	}
	//fill result with values from s1 as long as values are not found in s2
	int size = s1.size();
	for (int i = 0; i < size; i++) {
		ItemType s;
		s1.get(i, s);
		if (!s2.contains(s)) {
			result.insert(s);
		}
	}
}