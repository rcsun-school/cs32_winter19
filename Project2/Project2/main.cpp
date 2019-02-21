#include "Set.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;


int main() {
#ifdef TEST_1
	Set a; //default constructor
	string s = "cheese";
	assert(a.empty() && a.size() == 0); //private data initialized
	assert(!a.get(0, s) && s == "cheese"); //get returns false on empty set and s is unchanged
	assert(a.insert("apple")); //assert that "apple" is successfully inserted into a
	assert(a.erase("apple")); //erase a set of one element
	assert(a.empty() && a.size() == 0); //private data should be updated. 
	a.insert("apricot");
	a.insert("banana");
	a.insert("durian");
	a.insert("coconut");
	assert(a.contains("banana")); //test if contains
	assert(!a.contains("mango")); //test if doesn't contain
	assert(!a.insert("durian")); //insert returns false if duplicate
	assert(!a.erase("mango")); //erase returns false if value isn't in set
	a.insert("aardvark");
	assert(a.get(0, s) && s == "aardvark"); //ensure that aardvark is placed in front
	a.erase("aardvark");
	Set b(a); //copy constructor
	string t = "ham";
	assert(a.get(3, s) && s == "durian"); //s is changed if pos is inbounds
	assert(b.get(3, t) && t == s); //t has the same values as a
	b.insert("grapefruit");
	assert(!a.contains("grapefruit")); // a and b are independent sets
	Set c;
	c = b; //assignment operator
	assert(c.get(4, s) && s == "grapefruit" && b.get(4, t) && t == s); //b and c have identical values
	assert(c.erase("apricot") && b.contains("apricot")); //c and b are independent sets
	a = a; //assignment operator on self
	assert(a.contains("banana") && a.size() == 4); //a isn't deleted by accident
	Set d;
	b.swap(d);
	assert(b.empty() && d.contains("grapefruit") && d.size() == 5); //sets switch data with one another
	d = c;
	d.insert("parrot");
	assert(!d.contains("apricot")); //previous data gets deleted
	Set e;
	e.insert("avocado");
	e.insert("broccoli");
	unite(e, c, b); //unite two sets with different values
	assert(b.contains("durian") && b.size() == c.size() + e.size());
	Set f(c);
	unite(f, c, f);
	assert(f.get(2, s) && s == "durian" && f.size() == 4); //s1 is the same as result
	subtract(b, e, d);
	assert(!d.contains("parrot") && d.size() == c.size()); //previous data successfully wiped in subtract


	Set z;
	Set y;
	Set x;
	z.insert("a");
	z.insert("b");
	z.insert("c");
	y.insert("1");
	y.insert("2");
	unite(z, z, x); //unite if s1 = s2
	string p;
	assert(x.get(0, p) && z.get(0, s) && s == p); //x should be identical to z
	subtract(z, z, x); //subtract if s1 = s2
	assert(x.empty()); //x should be empty
	unite(x, y, x); //unite with result = s1
	assert(x.get(1, p) && y.get(1, s) && p == s); //x should be identical to y
	subtract(x, y, x); //s1 = result
	assert(x.empty());//x should be empty
	unite(z, y, x);
	subtract(z, x, x);//s2 = result
	assert(x.empty());
	x.insert("almond");
	x.insert("bean");
	x.insert("carrot");
	x.insert("duck");
	subtract(x, x, x); //all three arguments are the same
	assert(x.empty()); 
	cout << "all tests passed" << endl;



#endif

#ifndef PART_1
	//Test 2, unsigned long
	Set a;
	assert(a.insert(100));
	for (int i = 2; i < 6; i++) {
		a.insert(i * 100);
	}
	assert(a.erase(300));
	assert(!a.erase(900));
	Set b;
	for (int i = -10; i < 0; i++) {
		b.insert(i);
	}
	unsigned long s;
	b.get(3, s);
	assert(s == -7);
	a.swap(b);
	assert(a.contains(-5));
	Set c;
	a.swap(c);
	assert(a.empty());
	Set d(c);
	assert(d.contains(-3) && d.contains(-6));
	assert(d.erase(-4));
	Set e;
	unite(b, d, e);
	assert(e.size() == 13);
	Set f;
	f = e;
	subtract(f, e, f);
	assert(f.empty());
	cout << "all tests passed";

#endif
}