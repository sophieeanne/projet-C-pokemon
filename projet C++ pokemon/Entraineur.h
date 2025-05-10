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

	virtual void ajouterAdversaireVaincu(const string& nomAdversaire) = 0;
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
		int i = 0;
		for (auto& pokemon : equipe) {
			cout << i + 1 << " : " << pokemon->getNom() << endl;
			i++;
		}

		int choix;
		bool entreeValide = false;
		while (!entreeValide) {
			try {
				cout << "Voici vos Pokemons, choisissez le numero de Pokemon avec qui vous voulez interagir : ";
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

		equipe[choix - 1]->afficherMessageInteraction();
	}

	

};
class Joueur : public Entraineur
{
private:
	vector<string> badges;
	int combatsGagnes;
	int combatsPerdus;
	vector<string> adversairesVaincus;

public:
	//constructeur par défaut
	Joueur() : Entraineur("", {}), badges(0), combatsGagnes(0), combatsPerdus(0), adversairesVaincus() {} // Constructeur par défaut
	//constructeur
	Joueur(string Nom, vector<Pokemon*> Equipe, vector<string> Badges, int CombatsGagnes, int CombatsPerdus) : Entraineur(Nom, Equipe), badges(Badges), combatsGagnes(CombatsGagnes), combatsPerdus(CombatsPerdus), adversairesVaincus() {}

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
	void setAdversairesVaincus(vector<string> Adversaires) {
		adversairesVaincus = Adversaires;
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
	vector<string> getAdversairesVaincus() const {
		return adversairesVaincus;
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

	void ajouterAdversaireVaincu(const string& nomAdversaire) override{
		if (find(adversairesVaincus.begin(), adversairesVaincus.end(), nomAdversaire) == adversairesVaincus.end()) {
			adversairesVaincus.push_back(nomAdversaire);
			cout << nomAdversaire << " ajoute a la liste des adversaires vaincus." << endl;
		}
	}

	void interagirAdversaire() {
		if (adversairesVaincus.empty()) {
			cout << "Vous n avez vaincu aucun adversaire pour l instant." << endl;
			return;
		}

		cout << "=== ADVERSAIRES VAINCUS ===" << endl;
		for (size_t i = 0; i < adversairesVaincus.size(); ++i) {
			cout << i + 1 << ") " << adversairesVaincus[i] << endl;
		}

		int choix;
		bool entreeValide = false;
		while (!entreeValide) {
			try {
				cout << "Voici vos adversaires vaincus, choisissez le numero avec qui vous voulez interagir : ";
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

		string nomAdversaire = adversairesVaincus[choix - 1];
		//messages des Leaders
		if (nomAdversaire == "Ignis") {
			cout << "Ignis: \"La flamme de la defaite brule encore en moi...\"" << endl;
		}
		else if (nomAdversaire == "Aqua") {
			cout << "Aqua: \"Les vagues de ton talent m ont submerge !\"" << endl;
		}
		else if (nomAdversaire == "Volt") {
			cout << "Volt: \"Je dois admettre que tu as ete... electrisant !\"" << endl;
		}
		else if (nomAdversaire == "Flora") {
			cout << "Flora: \"Meme la plus forte des plantes flechit devant toi!\"" << endl;
		}
		//message du maitre
		/*else if (nomAdversaire == "") {

		}*/
		//message d un joueur (le meme pour tous)
		else {
			cout << nomAdversaire << ":\"Tu m as bien battu ! A la prochaine, pour la revanche !\"" << endl;
		}
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

	void ajouterAdversaireVaincu(const string& nomAdversaire) override {
		
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