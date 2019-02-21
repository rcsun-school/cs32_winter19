#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;



// Remove the odd integers from li.
// It is acceptable if the order of the remaining even integers is not
// the same as in the original list.
void removeOdds(list<int>& li)
{
	list <int>::iterator it = li.begin();
	while (it != li.end()) {
		if (*it % 2 != 0) {
			it = li.erase(it);
			
		}
		else {
			it++;
		}
	}
}


void removeOdds(vector<int>& v) //2. Remember that making a dummy iterator to destroy a value doesn't work on vector
{
	vector <int>::iterator it = v.begin();
	while (it != v.end()) {
		if (*it % 2 != 0) {
			it = v.erase(it);
		}
		else {
			it++;
		}
	}
}

vector<int> destroyedOnes;

class Movie //3
{
public:
	Movie(int r) : m_rating(r) {}
	~Movie() { destroyedOnes.push_back(m_rating); }
	int rating() const { return m_rating; }
private:
	int m_rating;
};

// Remove the movies in li with a rating below 50 and destroy them.
// It is acceptable if the order of the remaining movies is not
// the same as in the original list.
void removeBad(list<Movie*>& li)
{
	list<Movie*>::iterator it = li.begin();
	while (it != li.end()) {
		if ((*it)->rating() < 50) {
			delete *it;
			it = li.erase(it);
		}
		else {
			it++;
		}
	}
}

void removeBad(vector<Movie*>& v)
{
	vector<Movie*>::iterator it = v.begin();
	while (it != v.end()) {
		if ((*it)->rating() < 50) {
			delete *it;
			it = v.erase(it);
		}
		else {
			it++;
		}
	}
}


void test()
{
	int a[8] = { 2, 8, 5, 6, 7, 3, 4, 1 };
	list<int> x(a, a + 8);  // construct x from the array
	assert(x.size() == 8 && x.front() == 2 && x.back() == 1);
	removeOdds(x);
	assert(x.size() == 4);
	vector<int> v(x.begin(), x.end());  // construct v from x
	sort(v.begin(), v.end());
	int expect[4] = { 2, 4, 6, 8 };
	for (int k = 0; k < 4; k++)
		assert(v[k] == expect[k]);

	int b[8] = { 2, 8, 5, 6, 7, 3, 4, 1 };
	vector<int> y(a, a + 8);  // construct x from the array
	assert(y.size() == 8 && y.front() == 2 && y.back() == 1);
	removeOdds(y);
	assert(y.size() == 4);
	sort(y.begin(), y.end());
	int expect2[4] = { 2, 4, 6, 8 };
	for (int k = 0; k < 4; k++)
		assert(y[k] == expect2[k]);
}

void testA()
{
	int a[8] = { 85, 80, 30, 70, 20, 15, 90, 10 };
	list<Movie*> x;
	for (int k = 0; k < 8; k++)
		x.push_back(new Movie(a[k]));
	assert(x.size() == 8 && x.front()->rating() == 85 && x.back()->rating() == 10);
	removeBad(x);
	assert(x.size() == 4 && destroyedOnes.size() == 4);
	vector<int> v;
	for (list<Movie*>::iterator p = x.begin(); p != x.end(); p++)
	{
		Movie* mp = *p;
		v.push_back(mp->rating());
	}
	// Aside:  In C++11, the above loop could be
	//     for (auto p = x.begin(); p != x.end(); p++)
	//     {
	//         Movie* mp = *p;
	//         v.push_back(mp->rating());
	//     }
	// or    
	//     for (auto p = x.begin(); p != x.end(); p++)
	//     {
	//         auto mp = *p;
	//         v.push_back(mp->rating());
	//     }
	// or    
	//     for (Movie* mp : x)
	//         v.push_back(mp->rating());
	// or    
	//     for (auto mp : x)
	//         v.push_back(mp->rating());
	sort(v.begin(), v.end());
	int expect[4] = { 70, 80, 85, 90 };
	for (int k = 0; k < 4; k++)
		assert(v[k] == expect[k]);
	sort(destroyedOnes.begin(), destroyedOnes.end());
	int expectGone[4] = { 10, 15, 20, 30 };
	for (int k = 0; k < 4; k++)
		assert(destroyedOnes[k] == expectGone[k]);
	for (list<Movie*>::iterator p = x.begin(); p != x.end(); p++)
		delete *p;
}


void testB()
{
	int a[8] = { 85, 80, 30, 70, 20, 15, 90, 10 };
	vector<Movie*> x;
	for (int k = 0; k < 8; k++)
		x.push_back(new Movie(a[k]));
	assert(x.size() == 8 && x.front()->rating() == 85 && x.back()->rating() == 10);
	removeBad(x);
	assert(x.size() == 4 && destroyedOnes.size() == 4);
	vector<int> v;
	for (int k = 0; k < 4; k++)
		v.push_back(x[k]->rating());
	sort(v.begin(), v.end());
	int expect[4] = { 70, 80, 85, 90 };
	for (int k = 0; k < 4; k++)
		assert(v[k] == expect[k]);
	sort(destroyedOnes.begin(), destroyedOnes.end());
	int expectGone[4] = { 10, 15, 20, 30 };
	for (int k = 0; k < 4; k++)
		assert(destroyedOnes[k] == expectGone[k]);
	for (vector<Movie*>::iterator p = x.begin(); p != x.end(); p++)
		delete *p;
}


int main()
{
	test();
	testB();
	cout << "Passed" << endl;
}