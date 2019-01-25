#include "Set.h"

Set::Set() : m_size(0), m_head(nullptr), m_tail(nullptr), m_empty(true) { //construct an empty Set

}

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

Set::~Set() {
	Node * p = m_head;
	while (p != m_head) {
		Node * killme = p;
		p = p->next;
		delete killme;
	}
}

Set& Set::operator=(const Set& other) {
	if (this == &other) {
		return *this;
	}
	//clear out existing values in receiving set
	Node * p = m_head;
	while (p != m_head) {
		Node * killme = p;
		p = p->next;
		delete killme;
	}
	m_size = 0;
	m_empty = other.m_empty;
	m_head = nullptr;
	if (other.size() == 1) {
		m_head = new Node;
		m_head->value = other.m_head->value;
		m_tail = m_head;
		m_size++;
		return *this;
	}
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

bool Set::insert(const ItemType& value) {
	if (this->contains(value)) {
		return false;
	}
	if (m_empty) {
		m_head = new Node;
		m_head->value = value;
		m_head->next = nullptr;
		m_size++;
		m_empty = false;
		return true;
	}
			Node * p = new Node;
			p->value = value;
	if (m_size == 1) {
		
		if (m_head->value > value) {
			p->next = m_head;
			m_head->previous = p;
			m_head->next = nullptr;
			m_tail = m_head;
			m_head = p;
		}
		else {
			m_head->next = p;
			p->previous = m_head;
			p->next = nullptr;
			m_tail = p;
		}
		m_size++;
		return true;
	}
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
	m_tail->next = p;
	p->previous = m_tail;
	p->next = nullptr;
	m_tail = p;
	m_size++;
	return true;

}


bool Set::erase(const ItemType& value) {
	if (!contains(value)) {
		return false;
	}
	if (m_size == 1) {
		m_head = nullptr;
		m_empty = true;
		m_size--;
		return true;
	}
	for (Node * ptr = m_head; ptr->next != nullptr; ptr = ptr->next) {
		if (ptr->next->value == value) {
			Node * killme = ptr->next;
			ptr->next = killme->next;
			killme->next->previous = ptr;
			delete killme;
			m_size--;
			return true;
		}
	}
	return true;
}

bool Set::contains(const ItemType& value) const {
	for (Node * ptr = m_head; ptr != nullptr; ptr = ptr->next) { 
		if (ptr->value == value) {
			return true;
		}
	}
	return false;
}

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


void Set::swap(Set& other) {
	Node * temp = other.m_head;
	other.m_head = m_head;
	m_head = temp;
}