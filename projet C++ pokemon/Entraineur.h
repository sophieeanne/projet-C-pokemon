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
	vector<Pokemon*> equipe;
public : 
	//constructeur par défaut
	Entraineur() : nom(""), equipe({}) {}

	//constructeur
	Entraineur(string Nom, vector<Pokemon*> Equipe) : nom(Nom), equipe(Equipe) {} 

	//destructeur
	virtual ~Entraineur() = 0 {}
	//setters
	void setNom(string Nom) {
		nom = Nom;
	}
	//getters
	string getNom() {
		return nom;
	}
	vector<Pokemon*> getEquipe() {
		return equipe;
	}

	//méthodes
	void ajouterPokemon(const string& nomPokemon, const map<string, Pokemon*>& pokedex) {
		auto it = pokedex.find(nomPokemon);
		equipe.push_back(it->second);
		cout << nomPokemon << " a ete ajoute a l'equipe !" << endl;
	}

	void afficherEquipe() {
		cout << "=== MON EQUIPE ===" << endl;
		for (int i = 0; i < equipe.size(); i++) {
			equipe[i]->afficherInfos();
		}
	}
	bool equipePleine() {
		if (equipe.size() >= 6) {
			return true;
		}
		return false;
	}
	Pokemon* getPokemonActif() {
		for (auto& pokemon : equipe) {
			if (!pokemon->estKo()) {
				return pokemon;
			}
		}
		return nullptr; // Tous les Pokémon sont KO
	}
	void soignerPokemon() {
		for (auto& pokemon : equipe) {
			if (pokemon->estKo()) {
				pokemon->setHp(100); // Remettre les HP à 100
				cout << pokemon->getNom() << " a ete soigne !" << endl;
			}
			else {
				cout << "Tous les Pokemon sont en bonne sante !" << endl;
			}
		}
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
	Joueur(string Nom, vector<Pokemon*> Equipe, int Badges, int CombatsGagnes, int CombatsPerdus) : Entraineur(Nom, Equipe), badges(Badges), combatsGagnes(CombatsGagnes), combatsPerdus(CombatsPerdus) {}

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
		cout << "Combats gagnes : " << combatsGagnes << endl;
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
	LeaderGym(string Nom, vector<Pokemon*> Equipe, string BadgeGym, string NomGym) : Entraineur(Nom, Equipe), badgeGym(BadgeGym), nomGym(NomGym) {}
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
	MaitrePokemon(string Nom, vector<Pokemon*> Equipe) : Entraineur(Nom, Equipe) {}
	//destructeur
	~MaitrePokemon() {}

	//méthode
	double appliquerBonus(double degat) {
		return degat * 2.5;
	}
};