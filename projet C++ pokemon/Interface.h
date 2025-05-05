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
	map<string, Pokemon*> pokedex;
	string pokemonFichier = "pokemon.csv";
	unique_ptr<Entraineur> joueurActif;
	string fichierJoueurs = "joueur.csv";

	//constructeur par défaut
	Interface() : joueurActif(nullptr) {}
	//constructeur
	Interface(string NomFichier, string pf, map<string, Pokemon*> p) : fichier(NomFichier), pokemonFichier(pf), pokedex(p), joueurActif(nullptr) {
	}

	static Joueur creerNouveauJoueur(const map<string, Pokemon*>& pokedex, const string& fichier) {
		string nom;
		cout << "Entrez le nom du joueur : ";
		cin >> nom;

		Joueur nouveau(nom, {}, 0, 0, 0);
		
		cout << "Initialisation de l'équipe de 6 Pokémon :\n";
		for (int i = 0; i < 6; ++i) {
			string nomPoke;
			cout << "Nom du Pokémon #" << i + 1 << " : ";
			cin >> nomPoke;
			nouveau.ajouterPokemon(nomPoke, pokedex);
		}
		Interface::enregistrerJoueurDansFichier(nouveau, fichier); 
		return nouveau;
	}
	
	static void enregistrerJoueurDansFichier(Joueur& joueur, const string& nomFichier) {
		ofstream outFile(nomFichier, ios::app);
		if (!outFile) {
			cerr << "Erreur d'ouverture du fichier " << nomFichier << endl;
			return;
		}

		outFile << joueur.getNom();

		// Ajouter les noms des Pokémon (jusqu’à 6)
		const auto& equipe = joueur.getEquipe();
		for (int i = 0; i < 6; ++i) {
			if (i < equipe.size()) {
				outFile << "," << equipe[i]->getNom();
			}
			else {
				outFile << ","; // case vide si pas de Pokémon
			}
		}

		// Ajouter ensuite les nouveaux attributs
		outFile << "," << joueur.getBadges()
			<< "," << joueur.getCombatsGagnes()
			<< "," << joueur.getCombatsPerdus()
			<< endl;

		outFile.close();
	}

	static vector<Joueur> chargerJoueursDepuisFichier(const string& nomFichier, const map<string, Pokemon*>& pokedex) {
		vector<Joueur> joueurs;
		ifstream inFile(nomFichier);

		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << nomFichier << endl;
			return joueurs;
		}

		string ligne;
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			// Lire tous les champs de la ligne
			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (champs.size() < 1) continue;

			string nom = champs[0];
			vector<Pokemon*> equipe;  // Utiliser des pointeurs vers Pokémon

			// Les Pokémon vont des index 1 à 6 (inclus si présents)
			for (size_t i = 1; i <= 6 && i < champs.size(); ++i) {
				if (!champs[i].empty()) {
					auto it = pokedex.find(champs[i]);
					if (it != pokedex.end()) {
						equipe.push_back(it->second);  // Ajouter un pointeur vers le Pokémon
					}
				}
			}

			// Badges, Gagnes, Perdus si présents
			int badges = 0, gagnes = 0, perdus = 0;
			if (champs.size() >= 10) {
				badges = stoi(champs[7]);
				gagnes = stoi(champs[8]);
				perdus = stoi(champs[9]);
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
			joueurActif->afficherEquipe();
			break;
		case 2:
			joueurActif->soignerPokemon();
			break;
		case 3:
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
			AffronterJoueur();
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

	void AffronterJoueur() {
		cout << "Quel joueur voulez-vous affronter ?" << endl;
		string nomAdversaire;
		cin >> nomAdversaire;
		//on cherche le nom de l'adversaire dans le fichier joueur
		ifstream inFile(fichierJoueurs);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << fichierJoueurs << endl;
			return;
		}
		string ligne;
		bool joueurTrouve = false;
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			// Lire tous les champs de la ligne
			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (champs.size() < 1) continue;

			if (champs[0] == nomAdversaire) {
				joueurTrouve = true;
				break;
			}
		}
		inFile.close();
		if (!joueurTrouve) {
			cout << "Joueur non trouvé." << endl;
			return;
		}
		cout << "Vous allez affronter " << nomAdversaire << endl;
		Joueur adversaire;
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		if (joueur) {
			joueur->ajouterCombatGagne();
		}
		vector<Joueur> tousLesJoueurs = chargerJoueursDepuisFichier(fichierJoueurs, pokedex);
		for (auto j : tousLesJoueurs) {
			if (j.getNom() == nomAdversaire) {
				adversaire = j;
				break;
			}
		}
		int scoreJoueur = 0;
		int scoreAdversaire = 0;

		//On choisit au hasard qui commence (1 c'est le joueur, 2 c'est l'adversaire)
		int quiCommence = rand() % 2 + 1;

		//Boucle de combat : on continue jusqu'à qu'un joueur a mit 3 des pokémons de son adversaire KO
		while (scoreJoueur < 3 && scoreAdversaire < 3) {
			Pokemon* p1 = joueur->getPokemonActif();
			Pokemon* p2 = adversaire.getPokemonActif();

			if (p1 == nullptr || p2 == nullptr) {
				cout << "Un des joueurs n'a plus de Pokemon utilisables." << endl;
				break;
			}

			if (quiCommence == 1) {
				//joueur attaque
				cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
				p1->attaquer(*p2);
				p2->recevoirDegats(p1->calculerDegats(*p2));

				if (p2->estKo()) {
					cout << p2->getNom() << " est KO !" << endl;
					scoreJoueur++;
					if (scoreJoueur == 3) break;
				}

				//adversaire attaque (si son Pokémon n’est pas KO)
				if (!p2->estKo()) {
					cout << adversaire.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
					p2->attaquer(*p1);
					p1->recevoirDegats(p2->calculerDegats(*p1));

					if (p1->estKo()) {
						cout << p1->getNom() << " est KO !" << endl;
						scoreAdversaire++;
						if (scoreAdversaire == 3) break;
					}
				}
			}
			else {
				//adversaire commence
				cout << adversaire.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
				p2->attaquer(*p1);
				p1->recevoirDegats(p2->calculerDegats(*p1));

				if (p1->estKo()) {
					cout << p1->getNom() << " est KO !" << endl;
					scoreAdversaire++;
					if (scoreAdversaire == 3) break;
				}

				//joueur attaque (si son Pokémon n’est pas KO)
				if (!p1->estKo()) {
					cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
					p1->attaquer(*p2);
					p2->recevoirDegats(p1->calculerDegats(*p2));

					if (p2->estKo()) {
						cout << p2->getNom() << " est KO !" << endl;
						scoreJoueur++;
						if (scoreJoueur == 3) break;
					}
				}
			}
		}

		if (scoreJoueur == 3) {
			cout << joueurActif->getNom() << " a gagne le combat !" << endl;
			joueur->ajouterCombatGagne();
			adversaire.ajouterCombatPerdu();
		}
		else if (scoreAdversaire == 3) {
			cout << adversaire.getNom() << " a gagne le combat !" << endl;
			adversaire.ajouterCombatGagne();
			joueur->ajouterCombatPerdu();
		}
		else {
			cout << "Le combat est terminé." << endl;
		}




	}

	void Statistiques() {
		//modifier quand y'aura la classe Entraineur
	}
};

