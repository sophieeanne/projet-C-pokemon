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
	Interface(string NomFichier, string pf, map<string, Pokemon*> p, unique_ptr<Entraineur> jA) : fichier(NomFichier), pokemonFichier(pf), pokedex(p) {
		joueurActif = move(jA);
	}

	//setters
	void setJoueurActif(unique_ptr<Entraineur> jA) {
		joueurActif = move(jA);
	}
	
	//LES METHODES POUR GERER LES JOUEURS
	static Joueur creerNouveauJoueur(const map<string, Pokemon*>& pokedex, const string& fichier) {
		string nom;
		cout << "Entrez le nom du joueur : ";
		cin >> nom;

		Joueur nouveau(nom, {}, 0, 0, 0);
		//on affiche le nom des pokemons
		cout << "=== POKEMONS DISPONIBLES ===" << endl;
		for (const auto& pair : pokedex) {
			cout << pair.first << ", ";
		}
		cout << endl;
		cout << "Initialisation de l'equipe de 6 Pokemon :\n";
		int i = 0;
		while (i < 6) {
			string nomPokemon;
			cout << "Entrez le nom du Pokemon " << i + 1 << " : ";
			cin >> nomPokemon;

			//vérifier si le Pokémon existe dans le Pokédex
			auto it = pokedex.find(nomPokemon);
			if (it == pokedex.end()) {
				cout << "Le Pokemon '" << nomPokemon << "' n'existe pas dans le Pokedex !" << endl;
				continue;
			}
			bool dejaDansEquipe = false;
			for (const auto& p : nouveau.getEquipe()) {
				if (p->getNom() == nomPokemon) {
					dejaDansEquipe = true;
					break;
				}
			}

			if (dejaDansEquipe) {
				cout << "Ce Pokemon est deja dans votre equipe. Choisissez-en un autre." << endl;
				continue;
			}
			nouveau.ajouterPokemon(nomPokemon, pokedex);
			i++;
		}
		Interface::enregistrerJoueurDansFichier(nouveau, fichier); 
		cout << "Le joueur "<<nom<<" a ete cree avec succes !" << endl;
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
		getline(inFile, ligne); //ignorer la première ligne (en-tête)
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (champs.size() < 1) continue;

			string nom = champs[0];
			vector<Pokemon*> equipe;  

			for (size_t i = 1; i <= 6 && i < champs.size(); ++i) {
				if (!champs[i].empty()) {
					auto it = pokedex.find(champs[i]);
					if (it != pokedex.end()) {
						equipe.push_back(it->second);  
					}
				}
			}

			// Badges, Gagnes, Perdus si présents
			int badges = 0, gagnes = 0, perdus = 0;
			try {
				if (champs.size() >= 10) {
					if (!champs[7].empty()) badges = std::stoi(champs[7]);
					if (!champs[8].empty()) gagnes = std::stoi(champs[8]);
					if (!champs[9].empty()) perdus = std::stoi(champs[9]);
				}
			}
			catch (const std::invalid_argument& e) {
				cerr << "Erreur de conversion pour le joueur '" << champs[0] << "'. Valeur invalide dans les colonnes numériques." << endl;
				continue; 
			}


			Joueur joueur(nom, equipe, badges, gagnes, perdus);
			joueurs.push_back(joueur);
		}

		inFile.close();
		return joueurs;
	}

	static unique_ptr<Joueur> choisirJoueurActif(const string& fichierJoueurs, const map<string, Pokemon*>& pokedex) {
		cout << "Saisissez votre pseudo : ";
		string pseudo;
		cin >> pseudo;

		vector<Joueur> joueurs = chargerJoueursDepuisFichier(fichierJoueurs, pokedex);
		for ( auto& joueur : joueurs) {
			if (joueur.getNom() == pseudo) {
				cout << "Bienvenue " << joueur.getNom() << " !" << endl;
				unique_ptr<Joueur> joueurPtr = make_unique<Joueur>(joueur); 
				return joueurPtr; 
			}
		}

		return nullptr;
	}

	static void updateJoueurDansFichier(Joueur& joueur, const string& nomFichier) {
		ifstream inFile(nomFichier);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier : " << nomFichier << endl;
			return;
		}

		vector<string> lignes;
		string ligne;

		// Lire toutes les lignes et mettre à jour celle du joueur concerné
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (!champs.empty() && champs[0] == joueur.getNom()) {
				// Mise à jour des statistiques : Badges, CombatsGagnés, CombatsPerdus
				if (champs.size() >= 10) {
					champs[7] = to_string(joueur.getBadges());
					champs[8] = to_string(joueur.getCombatsGagnes());
					champs[9] = to_string(joueur.getCombatsPerdus());
				}
				else {
					cerr << "Format de ligne invalide pour le joueur : " << joueur.getNom() << endl;
				}
			}

			// Recomposer la ligne et l'ajouter à la liste
			string nouvelleLigne;
			for (size_t i = 0; i < champs.size(); ++i) {
				nouvelleLigne += champs[i];
				if (i != champs.size() - 1) nouvelleLigne += ",";
			}
			lignes.push_back(nouvelleLigne);
		}
		inFile.close();

		// Réécrire le fichier avec les données mises à jour
		ofstream outFile(nomFichier);
		if (!outFile) {
			cerr << "Erreur d'écriture dans le fichier : " << nomFichier << endl;
			return;
		}

		for (const auto& l : lignes) {
			outFile << l << endl;
		}
		outFile.close();
	}	



	//LE MENU PRINCIPAL
	void Menu() {
		int choix;
		do {
			cout << endl;
			cout << "=== MENU PRINCIPAL ===" << endl;
			cout << "1) Gerer mon equipe" << endl;
			cout << "2) Combattre" << endl;
			cout << "3) Statistiques" << endl;
			cout << "4) Quitter" << endl;
			cin >> choix;

			switch (choix) {
			case 1:
				GererEquipe();
				break;
			case 2:
				Combattre();
				break;
			case 3:
				Statistiques();
				break;
			case 4:
				cout << "Au revoir !" << endl;
				break;
			default:
				cout << "Choix invalide. Veuillez reessayer." << endl;
				break;
			}
		} while (choix != 4);	
	}
	
	//OK !
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

	//OK !	
	void Combattre() {
		cout << "=== COMBATTRE ===" << endl;
		cout << "1) Affronter un joueur" << endl;
		cout << "2) Defi de gymnase" << endl;
		cout << "3) Maitre Pokemon (debloque apres 8 badges)" << endl;
		cout << "4) Retour" << endl;
		int choix;
		cin >> choix;
		do {
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
				cout << "Choix invalide. Veuillez reessayer." << endl;
				break;
			}
		} while (choix < 1 || choix>4);
	}

	//OK ! 
	void AffronterJoueur() {
		vector<string> nomsJoueurs;
		ifstream inFile(fichierJoueurs);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << fichierJoueurs << endl;
			return;
		}

		string ligne;
		getline(inFile, ligne);
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			getline(ss, champ, ','); 
			if (!champ.empty()) {
				nomsJoueurs.push_back(champ);
			}
		}
		inFile.close();

		cout << "=== Joueurs disponibles ===" << endl;
		for (const string& nom : nomsJoueurs) {
			cout << "- " << nom << endl;
		}

		string nomAdversaire;
		bool joueurTrouve = false;

		do {
			cout << "Quel joueur voulez-vous affronter ? ";
			cin >> nomAdversaire;

			for (const string& nom : nomsJoueurs) {
				if (nom == nomAdversaire) {
					joueurTrouve = true;
					break;
				}
			}

			if (!joueurTrouve) {
				cout << "Joueur non trouvé. Veuillez réessayer." << endl;
			}
		} while (!joueurTrouve);

		cout << "Vous allez affronter " << nomAdversaire << " !" << endl;

		Joueur adversaire;
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		vector<Joueur> tousLesJoueurs = chargerJoueursDepuisFichier(fichierJoueurs, pokedex);
		for (auto j : tousLesJoueurs) {
			if (j.getNom() == nomAdversaire) {
				adversaire = j;
				break;
			}
		}


		int scoreJoueur = 0;
		int scoreAdversaire = 0;

		bool tourDuJoueur = rand() % 2 == 0;
		int round = 1;

		//boucle de combat : on continue jusqu'à qu'un joueur a mit 3 des pokémons de son adversaire KO
		while (scoreJoueur < 3 && scoreAdversaire < 3) {
			cout << "\n===== ROUND " << round << " =====" << endl;
			Pokemon* p1 = joueur->getPokemonActif();
			Pokemon* p2 = adversaire.getPokemonActif();

			if (p1 == nullptr || p2 == nullptr) {
				cout << "Un des joueurs n'a plus de Pokemon utilisables." << endl;
				break;
			}

			if (tourDuJoueur) {
				//joueur attaque
				cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
				p1->attaquer(*p2);
				p2->recevoirDegats(p1->calculerDegats(*p2));
				//PauseConsole();

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
					//PauseConsole();

					if (p1->estKo()) {
						cout << p1->getNom() << " est KO !" << endl;
						//PauseConsole();
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
				//PauseConsole();

				if (p1->estKo()) {
					cout << p1->getNom() << " est KO !" << endl;
					scoreAdversaire++;
					//PauseConsole();
					if (scoreAdversaire == 3) break;
				}

				//joueur attaque (si son Pokémon n’est pas KO)
				if (!p1->estKo()) {
					cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
					p1->attaquer(*p2);
					p2->recevoirDegats(p1->calculerDegats(*p2));
					//PauseConsole();

					if (p2->estKo()) {
						cout << p2->getNom() << " est KO !" << endl;
						scoreJoueur++;
						//PauseConsole();
						if (scoreJoueur == 3) break;
					}
				}
			}
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire << endl;
			PauseConsole();
			round++;
			tourDuJoueur = !tourDuJoueur;
		}

		cout << endl;
		cout << "=== FIN DU COMBAT ===" << endl;
		PauseConsole();
		//fin du combat : on affiche les scores et qui a gagné
		if (scoreJoueur == 3) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire << endl;

			cout << joueurActif->getNom() << " a gagne le combat !" << endl;

			cout << joueur->getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << adversaire.getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			joueur->ajouterCombatGagne();
			adversaire.ajouterCombatPerdu();

			updateJoueurDansFichier(*joueur, fichierJoueurs);
			updateJoueurDansFichier(adversaire, fichierJoueurs);
		}
		else if (scoreAdversaire == 3) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire << endl;
			cout << adversaire.getNom() << " a gagne le combat !" << endl;

			cout << adversaire.getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << joueur->getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			adversaire.ajouterCombatGagne();
			joueur->ajouterCombatPerdu();

			updateJoueurDansFichier(*joueur, fichierJoueurs);
			updateJoueurDansFichier(adversaire, fichierJoueurs);
		}
		else {
			cout << "Le combat est terminé." << endl;
		}

	}

	//OK !
	void Statistiques() {
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		if (joueur) {
			joueur->afficherStatistiques();
		}
		else {
			cout << "Aucun joueur actif !" << endl;
		}
	}

	//OK !
	void PauseConsole() {
		cout << "Appuyez sur une touche pour continuer..." << endl;
		cin.ignore();
		cin.get();
	}

};

