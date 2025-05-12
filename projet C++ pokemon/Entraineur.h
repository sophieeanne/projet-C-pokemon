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

	/// <summary>
	/// La méthode ajouterPokemon permet d'ajouter un Pokemon à l'équipe de l'entraîneur.
	/// </summary>
	/// <param name="nomPokemon">le nom du pokemon</param>
	/// <param name="pokedex">Le pokedex</param>
	void ajouterPokemon(const string& nomPokemon, const map<string, Pokemon*>& pokedex) {
		auto it = pokedex.find(nomPokemon);
		equipe.push_back(it->second);
		cout << nomPokemon << " a ete ajoute a l'equipe !" << endl;
	}

	/// <summary>
	/// Pour afficher l'équipe de l'entraîneur.
	/// </summary>
	void afficherEquipe() {
		cout << "=== MON EQUIPE ===" << endl;
		for (int i = 0; i < equipe.size(); i++) {
			equipe[i]->afficherInfos();
		}
	}
	/// <summary>
	/// Vérifie si l'équipe de l'entraîneur est pleine (6 Pokémon maximum).
	/// </summary>
	/// <returns>True si elle est pleine, false sinon</returns>
	bool equipePleine() {
		if (equipe.size() >= 6) {
			return true;
		}
		return false;
	}
	/// <summary>
	/// Recupere le pokemon actif pour voir si il est en etat de se battre.
	/// </summary>
	/// <returns>Le pokemon s'il n'est pas KO, un pointeur null si tous les pokemons sont ko</returns>
	Pokemon* getPokemonActif() {
		for (auto& pokemon : equipe) {
			if (!pokemon->estKo()) {
				return pokemon;
			}
		}
		return nullptr; //tous les pokemons sont KO
	}

	/// <summary>
	/// Méthode pour soigner les Pokémon de l'équipe de l'entraîneur.
	/// </summary>
	void soignerPokemon() {
		for (auto& pokemon : equipe) {
			if (pokemon->estKo()) {
				pokemon->setHp(pokemon->getHpMax()); //remettre les HP à leur maximum
				cout << pokemon->getNom() << " a ete soigne !" << endl;
			}
			else {
				cout << pokemon->getNom() << " n'a pas besoin de soins." << endl ;
			}
		}
		cout << "Tous les Pokemon sont en bonne sante !" << endl;
	}

	/// <summary>
	///	Methode pour changer l'ordre des Pokémon dans l'équipe de l'entraîneur.
	/// </summary>
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

	/// <summary>
	/// Méthode pour interagir avec un Pokémon de l'équipe de l'entraîneur.
	/// </summary>
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

	/// <summary>
	/// Cette fonction affiche les statistques du joueur (nombre de badges, combats gagnés et perdus).
	/// </summary>
	void afficherStatistiques() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badges : " << nbBadges() << endl;
		cout << "Combats gagnes : " << combatsGagnes << endl;
		cout << "Combats perdus : " << combatsPerdus << endl;
	}

	/// <summary>
	/// La fonction ajoute le badge du joueur.
	/// </summary>
	/// <param name="badge">L enom du badge gagné</param>
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

	/// <summary>
	/// Ajouter un combat gagné
	/// </summary>
	void ajouterCombatGagne() {
		combatsGagnes++;
	}
	/// <summary>
	/// Ajouter un combat perdu
	/// </summary>
	void ajouterCombatPerdu() {
		combatsPerdus++;
	}

	/// <summary>
	/// Ajouter un adversaire vaincu à la liste des adversaires vaincus.
	/// </summary>
	/// <param name="nomAdversaire">Nom de l'adversaire</param>
	void ajouterAdversaireVaincu(const string& nomAdversaire) override{
		if (find(adversairesVaincus.begin(), adversairesVaincus.end(), nomAdversaire) == adversairesVaincus.end()) {
			adversairesVaincus.push_back(nomAdversaire);
			/*cout << nomAdversaire << " ajoute a la liste des adversaires vaincus de " << nom << endl;*/
		}
	}

	/// <summary>
	/// Méthode pour charger la liste des adversaires vaincus depuis un fichier.
	/// </summary>
	/// <param name="nomFichier">Nom du fichier</param>
	/// <param name="nomJoueur">Nom du joueur</param>
	void chargerAdversairesVaincus(const string& nomFichier, const string& nomJoueur) {
		ifstream in(nomFichier);
		string ligne;
		while (getline(in, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;
			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}
			if (!champs.empty() && champs[0] == nomJoueur) {
				adversairesVaincus.assign(champs.begin() + 1, champs.end());
				break;
			}
		}
	}

	/// <summary>
	/// Methode pour sauvegarder un adversaire vaincu dans un fichier.
	/// </summary>
	/// <param name="nomJoueur">Nom du joueur qui a gagné</param>
	/// <param name="adversaire">Nom de l'adversaire qui a perdu</param>
	/// <param name="nomFichier">Nom du fichier</param>
	void sauvegarderAdversaireVaincu(const string& nomJoueur, const string& adversaire, const string& nomFichier) {
		ifstream inFile(nomFichier);
		vector<string> lignes;
		bool joueurTrouve = false;

		string ligne;
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (!champs.empty() && champs[0] == nomJoueur) {
				joueurTrouve = true;
				if (find(champs.begin() + 1, champs.end(), adversaire) == champs.end()) {
					champs.push_back(adversaire);  
				}
			}

			string nouvelleLigne;
			for (size_t i = 0; i < champs.size(); ++i) {
				nouvelleLigne += champs[i];
				if (i != champs.size() - 1) nouvelleLigne += ",";
			}
			lignes.push_back(nouvelleLigne);
		}
		inFile.close();

		if (!joueurTrouve) {
			lignes.push_back(nomJoueur + "," + adversaire);
		}

		ofstream outFile(nomFichier);
		for (const auto& l : lignes) {
			outFile << l << endl;
		}
		outFile.close();
	}



	/// <summary>
	/// Méthode pour interagir avec un adversaire vaincu.
	/// </summary>
	void interagirAdversaire() {
		chargerAdversairesVaincus("joueurs_vaincus.txt", nom); 
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
				if (cin.fail() || choix<1 || choix>adversairesVaincus.size()) {
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
		else if (nomAdversaire == "Regis") {
			cout << "Regis: \"Tu es un vrai maitre Pokemon ! Bravo !\"" << endl;
		}
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

	/// <summary>
	/// Méthode pour afficher les statistiques du gymnase.
	/// </summary>
	void afficherInfosGymnase() {
		cout << "=== STATISTIQUES ===" << endl;
		cout << "Badge : " << badgeGym << endl;
		cout << "Nom du gymnase : " << nomGym << endl;
	}

	/// <summary>
	/// Méthode pour interagir avec un adversaire vaincu (ne sert pas ici)
	/// </summary>
	/// <param name="nomAdversaire"></param>
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

	/// <summary>
	/// Méthode pour calculer les dégâts infligés par le Maitre Pokemon avec le bonus de 25%.
	/// </summary>
	/// <param name="degat">Le degat fait</param>
	/// <returns>Le nouveau degat avec le bonus</returns>
	double appliquerBonus(double degat) {
		return degat * 1.25;
	}

	/// <summary>
	/// Méthode pour ajouter les adversaires vaincus (ne sert pas ici)
	/// </summary>
	/// <param name="nomAdversaire"></param>
	void ajouterAdversaireVaincu(const string& nomAdversaire) override {
	}
};