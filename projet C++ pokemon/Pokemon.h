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
	int hpMax;

public:
	//Constructeurs
	Pokemon() : nom(""), type1(""), type2(""), hp(0) {} 
	Pokemon(string Nom, string t1, string t2, int Hp, string nomAttaque, int Degat) : nom(Nom), type1(t1), type2(t2), hp(Hp), hpMax(Hp){
		attaques[nomAttaque] = Degat;
	}
	Pokemon(string Nom, string t1, string t2, int Hp, string nomAttaque, int Degat, string Faiblesse) : nom(Nom), type1(t1), type2(t2), hp(Hp), hpMax(Hp), faiblesse(Faiblesse) {
		attaques[nomAttaque] = Degat;
	}

	//setters
	void setNom(string Nom) { nom = Nom; }
	void setType(string t1, string t2) {type1 = t1; type2 = t2;}
	void setHp(int Hp) { hp = Hp;}
	void setAttaque(string nomAttaque, int Degat) {attaques[nomAttaque] = Degat;}
	void setFaiblesse(string Faiblesse) {faiblesse = Faiblesse;}
	void setHpMax(int nvHp) { hpMax = nvHp; }

	//getters
	string getNom() const {return nom;}
	string getType1() const {return type1;}
	string getType2() const {return type2;}	
	int getHp() const {return hp;}
	string getFaiblesse() const {return faiblesse;}
	const map<string, int>& getAttaques() const {
		return attaques;
	}	
	int getHpMax() const { return hpMax; }
	
	

	//méthodes
	virtual void attaquer(Pokemon& cible) = 0;
	virtual int calculerDegats(Pokemon& cible) = 0;
	virtual void afficherMessageInteraction() = 0;
	virtual ~Pokemon() = default;

	/// <summary>
	/// Méthode qui décrémente les points de vie d'un pokémon lors qu'il est attaqué
	/// </summary>
	/// <param name="degats">le degat fait</param>
	void recevoirDegats(int degats) { hp = hp - degats; }

	/// <summary>
	/// Méthode qui affiche les informations d'un pokémon
	/// </summary>
	void afficherInfos() {
		cout << "Nom : " << nom << endl;
		cout << "Type : " << type1 << ", " << type2 << endl;
		if (hp < 0) {
			cout << "Hp : " << 0 << endl;
		}
		else {
			cout << "Hp : " << hp << endl;
		}
		cout << "Attaques : " << endl;
		for (const auto& attaque : attaques) {
			cout << "- " << attaque.first << " : " << attaque.second << " puissance" << endl;
		}
		cout << "Faiblesse : " << faiblesse << endl;
		cout << endl;
	}

	/// <summary>
	/// Méthode qui vérifie si un pokémon est KO
	/// </summary>
	/// <returns>Un booléen indicateur</returns>
	bool estKo() const {
		return hp <= 0;
	}

};


#endif

