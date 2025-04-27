#pragma once
#ifndef POKEMON
#define POKEMON

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "Attaque.h"
using namespace std;

class Pokemon {
protected:
    string nom;
	string type1;
	string type2;
    int hp;
	map<string, int> attaques;
    int degat;
    string faiblesse;

public:
	Pokemon(string Nom, string t1, string t2, int Hp, string nomAttaque, int Degat) : nom(Nom), type1(t1), type2(t2), hp(Hp){
		attaques[nomAttaque] = Degat;
	}
	//setters
	void setNom(string Nom) {
		nom = Nom;
	}
	void setType(string t1, string t2) {
		type1 = t1;
		type2 = t2;
	}
	void setHp(int Hp) {
		hp = Hp;
	}
	void setAttaque(string nomAttaque, int Degat) {
		attaques[nomAttaque] = Degat;
	}
	void setDegat(int Degat) {
		degat = Degat;
	}
	void setFaiblesse(string Faiblesse) {
		faiblesse = Faiblesse;
	}
	//getters
	string getNom() {
		return nom;
	}
	string getType1() {
		return type1;
	}
	string getType2() {
		return type2;
	}	
	int getHp() {
		return hp;
	}
	int getDegat() {
		return degat;
	}
	string getFaiblesse() {
		return faiblesse;
	}
	int getAttaque(string nomAttaque) {
		if (attaques.find(nomAttaque) != attaques.end()) {
			return attaques[nomAttaque];
		}
		return 0; // ou une valeur par défaut
	}
	
	//méthodes
	map<string, Pokemon> chargerPokemonDepuisFichier(const string& fichierNom) {
		map<string, Pokemon> pokedex;
		ifstream fichier(fichierNom);
		if (!fichier) {
			cerr << "Erreur lors de l'ouverture du fichier." << endl;
			return pokedex;
		}
		string ligne;
		getline(fichier, ligne); // Lire la première ligne (en-tête)
		while (getline(fichier, ligne)) {
			stringstream ss(ligne);
			string nom, type1, type2, hpStr, nomAttaque, degatStr;
			getline(ss, nom, ',');
			getline(ss, type1, ',');
			getline(ss, type2, ',');
			getline(ss, hpStr, ',');
			getline(ss, nomAttaque, ',');
			getline(ss, degatStr, ',');
			int hp = stoi(hpStr);
			int degat = stoi(degatStr);
			Pokemon poke(nom, type1, type2, hp, nomAttaque, degat);
			pokedex[nom] = poke;
		}
		return pokedex;
	}

	void afficherPokemon() {
		cout << "=== POKEMON ===" << endl;
		cout << "Nom : " << nom << endl;
		cout << "Type 1 : " << type1 << endl;
		cout << "Type 2 : " << type2 << endl;
		cout << "HP : " << hp << endl;
		cout << "Attaques : " << endl;
		for (const auto& attaque : attaques) {
			cout << "- " << attaque.first << ": " << attaque.second << endl;
		}
		cout << "Faiblesse : " << faiblesse << endl;
	}
};

#endif

