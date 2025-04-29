#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "Pokemon.h"
#include "Entraineur.h"
class Interface
{
public :
	string fichier;
	map<string, Pokemon> pokedex;
	string pokemonFichier = "pokemon.csv";
	unique_ptr<Entraineur> joueurActif;

	//constructeur par défaut
	Interface() : joueurActif(nullptr) {}
	//constructeur
	Interface(string NomFichier, string pf, map<string, Pokemon> p) : fichier(NomFichier), pokemonFichier(pf), pokedex(p), joueurActif(nullptr) {
	}

	static Joueur creerNouveauJoueur(const map<string, Pokemon>& pokedex, const string& fichier) {
		string nom;
		cout << "Entrez le nom du joueur : ";
		cin >> nom;

		Joueur nouveau(nom, {}, 0, 0, 0);

		int nb;
		cout << "Combien de Pokémon voulez-vous ajouter (max 6) ? ";
		cin >> nb;

		for (int i = 0; i < nb; ++i) {
			string nomPoke;
			cout << "Nom du Pokémon #" << i + 1 << " : ";
			cin >> nomPoke;
			nouveau.ajouterPokemon(nomPoke, pokedex);
		}
		Interface::enregistrerJoueurDansFichier(nouveau, fichier); 
		return nouveau;
	}
	
	static void enregistrerJoueurDansFichier(Joueur& joueur, const string& nomFichier) {
		ofstream outFile(nomFichier, ios::app); // ajouter à la fin
		if (!outFile) {
			cerr << "Erreur d'ouverture du fichier " << nomFichier << endl;
			return;
		}
		outFile << joueur.getNom() << ";"
			<< joueur.getBadges() << ";"
			<< joueur.getCombatsGagnes() << ";"
			<< joueur.getCombatsPerdus() << ";";

		for (int i = 0; i < joueur.getEquipe().size(); ++i) {
			outFile << joueur.getEquipe()[i].getNom();
			if (i != joueur.getEquipe().size() - 1) outFile << ",";
		}

		outFile << endl;
		outFile.close();
	}

	static vector<Joueur> chargerJoueursDepuisFichier(const string& nomFichier, const map<string, Pokemon>& pokedex) {
		vector<Joueur> joueurs;
		ifstream inFile(nomFichier);

		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << nomFichier << endl;
			return joueurs;
		}

		string ligne;
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string nom, badgesStr, gagnésStr, perdusStr, listePoke;

			getline(ss, nom, ';');
			getline(ss, badgesStr, ';');
			getline(ss, gagnésStr, ';');
			getline(ss, perdusStr, ';');
			getline(ss, listePoke, ';');

			int badges = stoi(badgesStr);
			int gagnes = stoi(gagnésStr);
			int perdus = stoi(perdusStr);

			vector<Pokemon> equipe;
			stringstream ssPoke(listePoke);
			string nomPoke;
			while (getline(ssPoke, nomPoke, ',')) {
				auto it = pokedex.find(nomPoke);
				if (it != pokedex.end()) {
					equipe.push_back(it->second);
				}
			}

			Joueur joueur(nom, equipe, badges, gagnes, perdus);
			joueurs.push_back(joueur);
		}

		inFile.close();
		return joueurs;
	}






	void GererEquipe() {
		if (!joueurActif) {
			cout << "Aucun joueur actif ! Veuillez démarrer un jeu." << endl;
			return;
		}
		cout << "=== GERER MON EQUIPE ===" << endl;
		cout << "1) Afficher mes pokemons" << endl;
		cout << "2) Soigner l'equipe" << endl;
		cout << "3) Retour" << endl;
		int choix;
		cin >> choix;
		if (choix < 1 || choix>5) {
			do {
				cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 4" << endl;

			} while (choix < 1 || choix>5);
		}
		switch (choix) {
		case 1:
			

			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}

	void Combattre() {
		cout << "=== COMBATTRE ===" << endl;
		cout << "1) Affronter un joueur" << endl;
		cout << "2) Defi de gymnase" << endl;
		cout << "3) Maitre Pokemon (debloque apres 8 badges)" << endl;
		cout << "4) Retour" << endl;
		int choix;
		cin >> choix;
		if (choix < 1 || choix>5) {
			do {
				cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 4" << endl;

			} while (choix < 1 || choix>5);
		}
		switch (choix) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}	

	void Statistiques() {
		//modifier quand y'aura la classe Entraineur
	}
};

