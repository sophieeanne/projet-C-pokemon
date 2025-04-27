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
	Entraineur(string Nom, vector<Pokemon> Equipe);

	//setters
	void setNom(string Nom) {
		nom = Nom;
	}
	//getters
	string getNom() {
		return nom;
	}
	void ajouterPokemon(Pokemon poke) {
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
	LeaderGym(string Nom, vector<Pokemon> Equipe, string BadgeGym, string NomGym) : Entraineur(Nom, Equipe), badgeGym(BadgeGym), nomGym(NomGym) {}

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

	void afficherInfosGymnase() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badge : " << badgeGym << endl;
		cout << "Nom du gymnase : " << nomGym << endl;
	}
};

class MaitrePokemon : public Entraineur
{
public : 
	double appliquerBonus(double degat) {
		return degat * 2.5;
	}
};