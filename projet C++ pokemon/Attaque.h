#pragma once
using namespace std;
#include <string>
class Attaque
{
private :
	string nom;
	string type;
	int puissance;
public:
	Attaque() {};
	Attaque(string n, string t, int p) {
		nom = n;
		type = t;
		puissance = p;
	}
	~Attaque() {};

	//getters
	string getNom() { return nom; }
	string getType() { return type; }
	int getPuissance() { return puissance; }

	//setters
	void setNom(string n) { nom = n; }
	void setType(string t) { type = t; }
	void setPuissance(int p) { puissance = p; }
};

