#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	struct GenomeData {
	public:
		string name;
		string position;
		int pos;
	};
	int m_minlength;
	Trie<GenomeData> m_trie;
	vector <Genome> m_genomes;
	bool containsGenome(GenomeData g, vector<DNAMatch> matches) const;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength): m_minlength(minSearchLength), m_trie()
{
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	m_genomes.push_back(genome);
	for (int i = 0; i < genome.length() - m_minlength; i++) {
		string s;
		if (genome.extract(i, m_minlength, s)) {
			GenomeData g;
			g.name = genome.name();
			g.position = "position " + char(i);
			g.pos = i;
			m_trie.insert(s, g);
		}

	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return m_minlength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	if (fragment.size < m_minlength || minimumLength < m_minlength) {
		return false;
	}
	if (!matches.empty()) {
		for (vector<DNAMatch>::iterator it = matches.begin(); it != matches.end(); it++) {
			it = matches.erase(it);
		}
	}
	//break fragment into pieces of the matcher's minimum length
	vector <GenomeData> total;
	for (int i = 0; i < fragment.size() - m_minlength; i++) {
		vector<GenomeData> store = m_trie.find(fragment.substr(i, m_minlength), &exactMatchOnly);
		for (vector<GenomeData>::iterator it = store.begin(); it != store.end(); it++) {
			total.push_back(*it);
		}
		delete[] &store;
	}
	for (vector<GenomeData>::iterator it = total.begin(); it != total.end(); it++) {
		if (!containsGenome(*it, matches)) {
			DNAMatch m;
			m.genomeName = it->name;
			m.position = it->pos;
			m.length = minimumSearchLength();
			matches.push_back(m);
		}

	}
	for (vector<DNAMatch>::iterator it = matches.begin(); it != matches.end(); it++) {
		if (it->length < minimumLength) {
			it = matches.erase(it);
		}
	} 
	if (matches.empty()) {
		return false;
	}
	return true;


} 

bool GenomeMatcherImpl::containsGenome(GenomeData g, vector<DNAMatch> matches) const {
	if (matches.empty()) {
		return false;
	}
	for (vector<DNAMatch>::iterator it = matches.begin(); it != matches.end(); it++) {
		if (it->genomeName == g.name) {
			if (g.pos == it->position + (it->length / minimumSearchLength()) + 1) {
				it->length++;
			}
			return true;
		}
	}
	return false;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
