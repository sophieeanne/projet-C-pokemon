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
	//constructeur par défaut
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

	//méthodes
	void ajouterPokemon(Pokemon poke) {
		if (poke.getNom() == "") {
			cout << "Le pokemon n'existe pas !" << endl;
			return;
		}
		if (equipePleine()) {
			return;
		}
		for (int i = 0; i < equipe.size(); i++) {
			if (equipe[i].getNom() == poke.getNom()) {
				cout << "Le pokemon est déjà dans l'équipe !" << endl;
				return;
			}
		}
		equipe.push_back(poke);
	}

	void afficherEquipe() {
		cout << "=== MON EQUIPE ===" << endl;
		for (int i = 0; i < equipe.size(); i++) {
			cout << i + 1 << ") " << equipe[i].getNom() << endl;
		}
	}

	bool equipePleine() {
		if (equipe.size() >= 6) {
			cout << "Votre equipe est pleine !" << endl;
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
	//constructeur par défaut
	Joueur() : Entraineur("", {}), badges(0), combatsGagnes(0), combatsPerdus(0) {} // Constructeur par défaut
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
		cout << "Combats gagnés : " << combatsGagnes << endl;
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
	//constructeur par défaut
	LeaderGym() : Entraineur("", {}), badgeGym(""), nomGym("") {} // Constructeur par défaut
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

	//méthodes
	void afficherInfosGymnase() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badge : " << badgeGym << endl;
		cout << "Nom du gymnase : " << nomGym << endl;
	}
};

class MaitrePokemon : public Entraineur
{
public : 
	//constructeur par défaut
	MaitrePokemon() : Entraineur("", {}) {} // Constructeur par défaut
	//constructeur
	MaitrePokemon(string Nom, vector<Pokemon> Equipe) : Entraineur(Nom, Equipe) {}
	//destructeur
	~MaitrePokemon() {}

	//méthode
	double appliquerBonus(double degat) {
		return degat * 2.5;
	}
};