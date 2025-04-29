#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <vector>
#include "Pokemon.h"
class Entraineur
{
protected : 
	string nom;
	vector<Pokemon> equipe;
public : 
	//constructeur par d�faut
	Entraineur() : nom(""), equipe({}) {}

	//constructeur
	Entraineur(string Nom, vector<Pokemon> Equipe) : nom(Nom), equipe(Equipe) {} 

	//destructeur
	~Entraineur() {}
	//setters
	void setNom(string Nom) {
		nom = Nom;
	}
	//getters
	string getNom() {
		return nom;
	}
	vector<Pokemon> getEquipe() {
		return equipe;
	}

	//m�thodes
	void ajouterPokemon(const string& nomPokemon, const map<string, Pokemon>& pokedex) {
		//v�rifier si le Pok�mon existe dans le Pok�dex
		auto it = pokedex.find(nomPokemon);
		if (it == pokedex.end()) {
			cout << "Le Pok�mon '" << nomPokemon << "' n'existe pas dans le Pok�dex !" << endl;
			return;
		}
		//v�rifier si l'�quipe est pleine
		if (equipePleine()) {
			cout << "L'�quipe est d�j� compl�te !" << endl;
			return;
		}
		//v�rifier si le Pok�mon est d�j� dans l'�quipe
		for (const auto& p : equipe) {
			if (p.getNom() == nomPokemon) {
				cout << "Le Pok�mon est d�j� dans l'�quipe !" << endl;
				return;
			}
		}
		//ajouter le Pok�mon � l'�quipe
		equipe.push_back(it->second);
		cout << nomPokemon << " a �t� ajout� � l'�quipe !" << endl;
	}

	void afficherEquipe() {
		cout << "=== MON EQUIPE ===" << endl;
		for (int i = 0; i < equipe.size(); i++) {
			cout << i + 1 << ") " << equipe[i].getNom() << endl;
		}
	}

	bool equipePleine() {
		if (equipe.size() >= 6) {
			return true;
		}
		return false;
	}
};
class Joueur : public Entraineur
{
private:
	int badges;
	int combatsGagnes;
	int combatsPerdus;
public:
	//constructeur par d�faut
	Joueur() : Entraineur("", {}), badges(0), combatsGagnes(0), combatsPerdus(0) {} // Constructeur par d�faut
	//constructeur
	Joueur(string Nom, vector<Pokemon> Equipe, int Badges, int CombatsGagnes, int CombatsPerdus) : Entraineur(Nom, Equipe), badges(Badges), combatsGagnes(CombatsGagnes), combatsPerdus(CombatsPerdus) {}

	//setters
	void setBadges(int Badges) {
		badges = Badges;
	}
	void setCombatsGagnes(int CombatsGagnes) {
		combatsGagnes = CombatsGagnes;
	}
	void setCombatsPerdus(int CombatsPerdus) {
		combatsPerdus = CombatsPerdus;
	}
	//getters
	int getBadges() {
		return badges;
	}
	int getCombatsGagnes() {
		return combatsGagnes;
	}
	int getCombatsPerdus() {
		return combatsPerdus;
	}

	void afficherStatistiques() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badges : " << badges << endl;
		cout << "Combats gagn�s : " << combatsGagnes << endl;
		cout << "Combats perdus : " << combatsPerdus << endl;
	}

	void ajouterBadge() {
		badges++;
	}
	void ajouterCombatGagne() {
		combatsGagnes++;
	}
	void ajouterCombatPerdu() {
		combatsPerdus++;
	}
};
class LeaderGym : public Entraineur
{
private : 
	string badgeGym;
	string nomGym;
public:
	//constructeur par d�faut
	LeaderGym() : Entraineur("", {}), badgeGym(""), nomGym("") {} // Constructeur par d�faut
	//constructeur
	LeaderGym(string Nom, vector<Pokemon> Equipe, string BadgeGym, string NomGym) : Entraineur(Nom, Equipe), badgeGym(BadgeGym), nomGym(NomGym) {}
	//destructeur
	~LeaderGym() {}

	//setters
	void setBadgeGym(string BadgeGym) {
		badgeGym = BadgeGym;
	}
	void setNomGym(string NomGym) {
		nomGym = NomGym;
	}
	//getters
	string getBadgeGym() {
		return badgeGym;
	}
	string getNomGym() {
		return nomGym;
	}

	//m�thodes
	void afficherInfosGymnase() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badge : " << badgeGym << endl;
		cout << "Nom du gymnase : " << nomGym << endl;
	}
};

class MaitrePokemon : public Entraineur
{
public : 
	//constructeur par d�faut
	MaitrePokemon() : Entraineur("", {}) {} // Constructeur par d�faut
	//constructeur
	MaitrePokemon(string Nom, vector<Pokemon> Equipe) : Entraineur(Nom, Equipe) {}
	//destructeur
	~MaitrePokemon() {}

	//m�thode
	double appliquerBonus(double degat) {
		return degat * 2.5;
	}
};