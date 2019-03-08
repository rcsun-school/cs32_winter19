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
		ValueType value;
		vector <Node *> child;
	};
	Node * m_root;
	void removeTree(Node * p);
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
		else {
			int size = p->child.size();
			for (int i = 0; i < size; i++) {
				removeTree(p->child[i]);
			}
		}
	}
	else {}
	delete *p;
}

template <typename ValueType>
inline
void Trie<ValueType>::reset() {
	delete this;
	&this = new Trie;
}
#endif // TRIE_INCLUDED
