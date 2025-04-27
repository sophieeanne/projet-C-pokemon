#pragma once
#ifndef POKEMON
#define POKEMON

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Pokemon {
protected:
    string nom;
	string type1;
	string type2;
    int hp;
	map<string, int> attaques;
    string faiblesse;

public:
	Pokemon() : nom(""), type1(""), type2(""), hp(0) {} // Constructeur par défaut
	Pokemon(string Nom, string t1, string t2, int Hp, string nomAttaque, int Degat) : nom(Nom), type1(t1), type2(t2), hp(Hp){
		attaques[nomAttaque] = Degat;
	}
	~Pokemon() {} // Destructeur

	//setters
	void setNom(string Nom) { nom = Nom; }
	void setType(string t1, string t2) {type1 = t1; type2 = t2;}
	void setHp(int Hp) { hp = Hp;}
	void setAttaque(string nomAttaque, int Degat) {attaques[nomAttaque] = Degat;}
	void setFaiblesse(string Faiblesse) {faiblesse = Faiblesse;}

	//getters
	string getNom() const {return nom;}
	string getType1() const {return type1;}
	string getType2() const {return type2;}	
	int getHp() const {return hp;}
	string getFaiblesse() const {return faiblesse;}
	const map<string, int>& getAttaques() const {
		return attaques;
	}	
	//méthodes

};

#endif

