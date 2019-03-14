#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
	struct Node {
	public: 
		char label;
		vector <ValueType> value;
		vector <Node *> child;
	};
	Node * m_root;
	void removeTree(Node *p);
	void trace(Node * root, std::string& key, bool exactMatchOnly, std::vector <ValueType>& result);
	
};

template <typename ValueType>
inline
Trie<ValueType>::Trie() {
	m_root = new Node;
}

template <typename ValueType>
inline 
Trie<ValueType>::~Trie() {
removeTree(m_root);
}

template <typename ValueType>
inline
void Trie<ValueType>::removeTree(Node * p) {
	if (p == nullptr) {
		return;
	}
	else if (!p->child.empty()) {
		int size = p->child.size();
		for (int i = 0; i < size; i++) {
			removeTree(p->child[i]);
		}
	}
	else {}
	delete *p;
}

template <typename ValueType>
inline
//free all memory, then allocate this to a new empty Trie
void Trie<ValueType>::reset() {
	delete this;
	&this = new Trie;
}

template <typename ValueType>
inline
//associates specified key with the specified value in the trie by adding appropriate nodes to the trie,
//then adding value at appropriate node 
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
	if (m_root->child.empty()) {
		Node n = new Node;
		n->label = key[0];
		n->next = nullptr;

	}
	int i = 0;
	Node * p = m_root;
	while (i < key.size()) {
		if (i < key.size() - 1 && p->child.empty()) { //if the trie doesn't have a child node at level i + `, fill a child node
			Node m = new Node;
			m->label = key[i];
			p->child.push_back(m);
			p = m;
		}
		for (vector<Node *>::iterator it = p->child.begin(); it != p->child.end(); it++) {
			if (key[i] == p->label) { //the label is already contained in a vector on the appropriate tier
				p = *it; // down a tier
				break;
			}
			else {
				Node n = new Node;
				n->label = key[i];
				p->child.push_back(n);
				p = n;
				if (i + 1 == key.size()) {
					n->value.push_back(value);
				}
				break;
			}
		}
		i++;
	}

}

template <typename ValueType>
inline
//if exactMatchOnly is true, return a vector containing all values associated w/ key string. No required ordering of returned valuesif no values found, return an empty vector.
//else, return a vector w/ all values associated with exact same search term and other keys off by one (barring the first term)
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
	if (key.size() == 0) {
		return;
	}
}

template <typename ValueType>
inline
void Trie<ValueType>::trace(Node * root, std::string& key, bool exactMatchOnly, std::vector<ValueType>& result) {
	if (key.size() == 0) {
		return; //base case, there are no characters to evaluate
	}
	Node next = nullptr;
	for (vector<Node*>iterator::it = root->child.begin(); it != root->child.end(); it++) {
		if ((*it->label == key[0])) {
			if ((*it)->value != nullptr) { //base case, found a match
				result.push_back((*it)->value);
				return;
			}
			next = it;
			break;
		}
	}

	if (key.size > 1) {
		if (next != nullptr) {
			trace(&next, key.substr(1, key.size() - 1), exactMatchOnly, result);
		}
		if (exactMatchOnly == false) { //off by one, so keep trying
			for (vector<Node*>iterator::it = root->child.begin(); it != root->child.end(); it++))
				trace(*it, key.substr(1, key.size() - 1), true, result);
		}
	}
}

#endif // TRIE_INCLUDED
