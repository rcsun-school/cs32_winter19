#include "Set.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;
int main() {
	Set a;
	a.insert("apple");
	string s;
	a.insert("banana");
	a.insert("coconut");
	a.insert("durian");
	a.insert("grape");
	a.insert("flan");
	a.erase("durian");
	Set b(a);
	Set c;
	c.insert("lemon");
	c.insert("cat");
	c = b;
	c.get(1, s);

	cout << s << endl;

}