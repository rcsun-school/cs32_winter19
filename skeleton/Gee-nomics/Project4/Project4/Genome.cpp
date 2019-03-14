#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence): m_name(nm), m_sequence(sequence)
{
    // This compiles, but may not be correct
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	string s;
	vector<string> names;
	vector <string> sequence;
	string temp;
	while (getline(genomeSource, s)) {
		if (s.size() > 80 || s.size() < 1) {
			return false;
		}
		if (s[0] == '>') {
			names.push_back(s.substr(1, s.size() - 1));
			if (temp.size() >= 0) {
				string undercase = "";
				for (int i = 0; i < temp.size(); i++) {
					char a = toupper(temp[i]);
					if (a != 'A' && a != 'T' && a != 'G' && a != 'C' && a != 'N') {
						return false;
					}
					undercase = a;
				}
				sequence.push_back(undercase);
				temp = "";
			}
		}
		else {
			temp += s;
		}
	}
	sequence.push_back(temp);
	for (int i = 0; i < names.size(); i++) {
		Genome * g = new Genome(names[i], sequence[i]);
		genomes.push_back(*g);
	}
	delete [] &names;
	delete [] &sequence;


		// This compiles, but may not be correct
}

int GenomeImpl::length() const
{
    return m_sequence.size();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
    return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position + length > this->length()) {
		return false;
	}
	fragment = m_sequence.substr(position, length);
	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
