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
				cout << pokemon->getNom() << " n a pas besoin de soins." << endl ;
			}
		}
		cout << "Tous les Pokemon sont en bonne sante !" << endl;
	}

	void changerOrdre() {
		int i = 0;
		for (auto& pokemon : equipe) {
			cout << i + 1 << " : " << pokemon->getNom() << endl;
			i++;
		}

		int choix;
		bool entreeValide = false;
		while (!entreeValide) {
			try {
				cout << "Choisissez le numero du Pokemon a changer : ";
				cin >> choix;
				if (cin.fail() || choix<1 || choix>equipe.size()) {
					throw out_of_range("Choix invalide.");
				}
				entreeValide = true;
			}
			catch (const exception& e) {
				cout << e.what() << "Veuillez reessayer." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}

		int choix2;
		entreeValide = false;
		while (!entreeValide) {
			try {
				cout << "A quel endroit voulez-vous le mettre ? (1-" << equipe.size() << ") : ";
				cin >> choix2;
				if (cin.fail() || choix2<1 || choix2>equipe.size()) {
					throw out_of_range("Choix invalide.");
				}
				entreeValide = true;
			}
			catch (const exception& e) {
				cout << e.what() << "Veuillez reessayer." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}

		swap(equipe[choix - 1], equipe[choix2 - 1]);
		cout << "Pokemon " << equipe[choix - 1]->getNom() << " a ete deplace !" << endl;
		i = 0;
		for (auto& pokemon : equipe) {
			cout << i + 1 << " : " << pokemon->getNom() << endl;
			i++;
		}
		
	}


	void interagirPokemon() {

	}
	void interagirAdversaire() {

	}

};
class Joueur : public Entraineur
{
private:
	vector<string> badges;
	int combatsGagnes;
	int combatsPerdus;
public:
	//constructeur par défaut
	Joueur() : Entraineur("", {}), badges(0), combatsGagnes(0), combatsPerdus(0) {} // Constructeur par défaut
	//constructeur
	Joueur(string Nom, vector<Pokemon*> Equipe, vector<string> Badges, int CombatsGagnes, int CombatsPerdus) : Entraineur(Nom, Equipe), badges(Badges), combatsGagnes(CombatsGagnes), combatsPerdus(CombatsPerdus) {}

	//setters
	void setBadges(vector<string> Badges) {
		badges = Badges;
	}
	void setCombatsGagnes(int CombatsGagnes) {
		combatsGagnes = CombatsGagnes;
	}
	void setCombatsPerdus(int CombatsPerdus) {
		combatsPerdus = CombatsPerdus;
	}
	//getters
	vector<string> getBadges() {
		return badges;
	}
	int getCombatsGagnes() {
		return combatsGagnes;
	}
	int getCombatsPerdus() {
		return combatsPerdus;
	}
	int nbBadges() {
		return badges.size();
	}

	//méthodes
	void afficherStatistiques() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badges : " << nbBadges() << endl;
		cout << "Combats gagnes : " << combatsGagnes << endl;
		cout << "Combats perdus : " << combatsPerdus << endl;
	}

	void ajouterBadge(string badge) {
		auto it = find(badges.begin(), badges.end(), badge);
		if (it == badges.end()) {
			badges.push_back(badge);
			cout << "Badge " << badge << " ajoute !" << endl;
		}
		else {
			cout << "Vous avez deja ce badge !" << endl;
		}
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