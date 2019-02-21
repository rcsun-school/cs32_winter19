class Landmark {
public:
	Landmark(string s);
	virtual ~Landmark();
	virtual string color() const;
	virtual string name() const;
	virtual string icon() const = 0;
private:
	string m_name;
};


class Restaurant : public Landmark {
public:
	Restaurant(string s, int num);
	virtual ~Restaurant();
	virtual string icon() const;
private:
	string m_name;
	int m_size;

};

class Hotel : public Landmark {
public:
	Hotel(string s);
	virtual ~Hotel();
	virtual string icon() const;
private:
	string m_name;
};

class Hospital : public Landmark {
public:
	Hospital(string s);
	virtual ~Hospital();
	virtual string color() const;
	virtual string icon() const;
private: 
	string m_name;
};

Landmark::Landmark(string s) {
	m_name = s;
}

Landmark::~Landmark() {

}

string Landmark::name() const {
	return m_name;
}

string Landmark::color() const {
	return "yellow";
}

Hotel::Hotel(string s) : Landmark(s) {
	
}

Hotel::~Hotel() {
	cout << "Destroying the hotel " << name() << "." << endl;
}

string Hotel::icon() const {
	return "bed";
}

Restaurant::Restaurant(string s, int num): Landmark(s),  m_size(num){

}

Restaurant::~Restaurant() {
	cout << "Destroying the restaurant " << name() << "." << endl;
}

string Restaurant::icon() const {
	if (m_size < 40)
		return "small knife/fork";
	else
		return "large knife/fork";
}

Hospital::Hospital(string s): Landmark(s){

}

Hospital::~Hospital() {
	cout << "Destroying the hospital " << name() << "." << endl;
}

string Hospital::color() const{
	return "blue";
}

string Hospital::icon() const {
	return "H";
}

