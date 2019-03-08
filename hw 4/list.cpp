

//delete everything except below function when done
//Print all the menu items 
void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m == nullptr) {
		return;
	}
	else if (m->name() == ""){
		for (int i = 0; i < m->menuItems()->size(); i++) {
			MenuItem * p = (*m->menuItems())[i];
			listAll(p, "");
		}
	}
	else {
		cout << path + m->name() << endl;
		if (m->menuItems() == nullptr) { //check to see there is a lower level
			return;
		}
		for (int i = 0; i < m->menuItems()->size(); i++) {
				MenuItem * p = (*m->menuItems())[i];
				listAll(p, path + m->name() + '/');
		}
	}
}
