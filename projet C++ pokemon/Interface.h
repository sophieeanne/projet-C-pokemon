#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>    
#include <chrono>     
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
	string fichierLeaders = "leaders.csv";
	string fichierMaitres = "maitres.csv";
	string listes_battus = "joueurs_vaincus.txt";

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

	/// <summary>
	/// Méthode pour créer un nouveau joueur.
	/// </summary>
	/// <param name="pokedex">Le pokedex</param>
	/// <param name="fichier">Le nom du fichier (ici le fichier joueurs)</param>
	/// <returns>Un objet joueur</returns>
	static Joueur creerNouveauJoueur(const map<string, Pokemon*>& pokedex, const string& fichier) {
		string nom;

		do {
			cout << "Entrez le nom du joueur : ";
			cin.ignore();
			getline(cin, nom);
			if (joueurExiste(nom, fichier)) {
				cout << "\"" << nom << "\" est deja pris. Veuillez en choisir un autre." << endl;
			}
		} while (joueurExiste(nom, fichier));
		

		vector<string> badgesGagnes;
		Joueur nouveau(nom, {}, badgesGagnes , 0, 0);
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

	/// <summary>
	/// Méthode pour vérifier si un joueur existe déjà dans le fichier.
	/// </summary>
	/// <param name="nom">Le nom du joueur</param>
	/// <param name="nomFichier">Nom du fichier</param>
	/// <returns>Vrai si le joueur existe, faux sinon</returns>
	static bool joueurExiste(const string& nom, const string& nomFichier) {
		ifstream inFile(nomFichier);
		if (!inFile) {
			cerr << "Erreur d'ouverture du fichier " << nomFichier << endl;
			return false;
		}
		string ligne;
		while (getline(inFile, ligne)) {
			istringstream iss(ligne);
			string nomJoueur;
			getline(iss, nomJoueur, ',');

			if (nomJoueur == nom) {
				inFile.close();
				return true;
			}

		}
		inFile.close();
		return false;
	}

	/// <summary>
	/// Methode pour enregistrer un joueur dans un fichier CSV.
	/// </summary>
	/// <param name="joueur">L'objet joueur</param>
	/// <param name="nomFichier">Le nom du fichier</param>
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
		const auto& badges = joueur.getBadges(); 
		string badgesString;
		for (size_t i = 0; i < badges.size(); ++i) {
			badgesString += badges[i];
			if (i < badges.size() - 1) {
				badgesString += '|';
			}
		}

		// Ajouter ensuite les nouveaux attributs
		outFile << "," << badgesString
			<< "," << joueur.getCombatsGagnes()
			<< "," << joueur.getCombatsPerdus()
			<< endl;

		outFile.close();
	}

	/// <summary>
	/// Méthode pour charger les joueurs depuis un fichier CSV.
	/// </summary>
	/// <param name="nomFichier">Le nom du fichier</param>
	/// <param name="pokedex">Le pokedex</param>
	/// <returns>Un vecteur avec tous les objets joueurs qui existent</returns>
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

			vector<string> badgesGagnes;
			if (champs.size() > 7 && !champs[7].empty()) {
				stringstream ssBadges(champs[7]);
				string badge;
				while (getline(ssBadges, badge, '|')) {
					badgesGagnes.push_back(badge);
				}
			}

			//Gagnes, Perdus si présents
			int combatsGagnes = 0, combatsPerdus = 0;
			try {
				if (champs.size() > 8 && !champs[8].empty())
					combatsGagnes = stoi(champs[8]);
				if (champs.size() > 9 && !champs[9].empty())
					combatsPerdus = stoi(champs[9]);
			}
			catch (const invalid_argument&) {
				cerr << "Erreur de conversion pour les stats de " << nom << endl;
				continue;
			}

			Joueur joueur(nom, equipe, badgesGagnes, combatsGagnes, combatsPerdus);
			joueurs.push_back(joueur);
		}

		inFile.close();
		return joueurs;
	}

	/// <summary>
	/// Methode pour choisir le joueur actif de la partie
	/// </summary>
	/// <param name="fichierJoueurs">Nom du fichier</param>
	/// <param name="pokedex">Le pokedex</param>
	/// <returns>Un pointeur du Joueur</returns>
	static unique_ptr<Joueur> choisirJoueurActif(const string& fichierJoueurs, const map<string, Pokemon*>& pokedex) {
		cout << "Saisissez votre pseudo : ";
		cin.ignore();
		string pseudo;
		getline(cin, pseudo);

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

	/// <summary>
	/// Methode pour actualiser les informations du joueur dans le fichier CSV.
	/// </summary>
	/// <param name="joueur">Le nom du joueur</param>
	/// <param name="nomFichier">Le nom du fichier</param>
	static void updateJoueurDansFichier(Joueur& joueur, const string& nomFichier) {
		ifstream inFile(nomFichier);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier : " << nomFichier << endl;
			return;
		}

		vector<string> lignes;
		string ligne;

		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (!champs.empty() && champs[0] == joueur.getNom()) {
				const auto& equipe = joueur.getEquipe();
				for (int i = 0; i < 6; ++i) {
					if (i < equipe.size()) {
						if (champs.size() > i + 1)
							champs[i + 1] = equipe[i]->getNom();
						else
							champs.push_back(equipe[i]->getNom());
					}
					else {
						if (champs.size() > i + 1)
							champs[i + 1] = "";
						else
							champs.push_back("");
					}
				}
				if (champs.size() >= 10) {
					const vector<string>& badges = joueur.getBadges();
					string badgesStr;
					for (size_t i = 0; i < badges.size(); ++i) {
						badgesStr += badges[i];
						if (i != badges.size() - 1) badgesStr += "|";
					}

					champs[7] = badgesStr;
					champs[8] = to_string(joueur.getCombatsGagnes());
					champs[9] = to_string(joueur.getCombatsPerdus());
				}
				else {
					cerr << "Format de ligne invalide pour le joueur : " << joueur.getNom() << endl;
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


	//LES METHODES POUR GERER LES LEADERSGYM

	/// <summary>
	/// Méthode pour promouvoir un joueur en Leader de Gym.
	/// </summary>
	/// <param name="joueur">L'objet joueur qui a vaincu</param>
	/// <param name="leaderBattu">Le nom du leader battu</param>
	/// <returns>Le joueur devenu leader</returns>
	LeaderGym promouvoirEnLeaderGym(Joueur& joueur, LeaderGym& leaderBattu) {
		map<string, int> typeCounts;

		//on compte le type de pokemon le plus présent dans l'équipe
		for (auto* p : joueur.getEquipe()) {
			if (p) {
				string t1 = p->getType1();
				string t2 = p->getType2();

				if (!t1.empty()) typeCounts[t1]++;
				if (!t2.empty()) typeCounts[t2]++;
			}
		}
		
		//on determine le type dominant
		string typeDominant = "";
		int maxCount = 0;
		for (const auto& pair : typeCounts) {
			if (pair.second > maxCount) {
				maxCount = pair.second;
				typeDominant = pair.first;
			}
		}

		//les leaders possibles et leurs gymnases
		map<string, pair<string, string>> gymData = {
			  {"Feu", {"Flamme", "Gym Feu"}},
			  {"Eau", {"Cascade", "Gym Eau"}},
			  {"Electrik", {"Volt", "Gym Electrik"}},
			  {"Feu", {"Flamme", "Volcano"}}
		};

		//on cherche le badge et le gymnase correspondant au type dominant
		string badge = "Badge Inconnu";
		string gym = "Gym Inconnu";
		if (gymData.find(typeDominant) != gymData.end()) {
			badge = gymData[typeDominant].first;
			gym = gymData[typeDominant].second;
		}

		//on crée le leader gym
		LeaderGym leader(joueur.getNom(), joueur.getEquipe(), badge, gym);
		ifstream fichier("leaders.csv");
		if (!fichier) {
			cerr << "Erreur lors de l'ouverture de leaders.csv" << endl;
			return leader;
		}

		vector<string> lignes;
		string ligne;

		bool leaderBattuTrouve = false;
		while (getline(fichier, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (!champs.empty() && champs[0] == leaderBattu.getNom()) {
				// Si on trouve le leader battu, on remplace la ligne par le nouveau leader
				string nouvelleLigne = leader.getNom() + "," + leaderBattu.getNomGym() + "," + leaderBattu.getBadgeGym();
				for (auto* p : leader.getEquipe()) {
					nouvelleLigne += "," + (p ? p->getNom() : "");
				}
				lignes.push_back(nouvelleLigne);
				leaderBattuTrouve = true;
			}
			else {
				lignes.push_back(ligne);
			}
		}
		fichier.close();
		if (!leaderBattuTrouve) {
			cerr << "Leader battu non trouvé : " << leaderBattu.getNom() << endl;
			return leader;
		}
		ofstream outFile("leaders.csv");
		if (!outFile) {
			cerr << "Erreur d'écriture dans le fichier leaders.csv" << endl;
			return leader;
		}
		for (const auto& l : lignes) {
			outFile << l << endl;
		}

		outFile.close();

		cout << joueur.getNom() << " a ete promu Leader de " << leaderBattu.getNomGym() << " avec le badge " << leaderBattu.getBadgeGym() << " !" << endl;
		return leader;
	}

	/// <summary>
	/// Methode pour charger les leaders de gym depuis un fichier CSV.
	/// </summary>
	/// <param name="fichier">Le nom du fichier (leaders.csv)</param>
	/// <param name="pokedex">Le pokedex</param>
	/// <returns>Un vecteur de tous les leaders qui existent</returns>
	static vector<LeaderGym> chargerLeaderGymdepuisFichier(const string& fichier, const map<string, Pokemon*>& pokedex) {
		vector<LeaderGym> leaders;
		ifstream inFile(fichier);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << fichier << endl;
			return leaders;
		}
		string ligne;
		getline(inFile, ligne); 
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (champs.size() < 3) continue;

			string nom = champs[0];
			string nomGym = champs[1];
			string badgeGym = champs[2];

			vector<Pokemon*> equipe;
			for (size_t i = 3; i < champs.size(); ++i) {
				if (!champs[i].empty()) {
					auto it = pokedex.find(champs[i]);
					if (it != pokedex.end()) {
						equipe.push_back(it->second);
					}
				}
			}

			LeaderGym leader(nom, equipe, badgeGym, nomGym);
			leaders.push_back(leader);
		}
	}


	//LES METHODES POUR GERER LES MAITRES POKEMON 

	/// <summary>
	/// Methode pour charger les maitres pokemons depuis un fichier CSV.
	/// </summary>
	/// <param name="nomFichier">Le nom du fichier (maitres.csv)</param>
	/// <returns>Un vecteur avec chaque leader</returns>
	vector<MaitrePokemon> chargerMaitresDepuisFichier(const string& nomFichier) {
		vector<MaitrePokemon> maitres;
		ifstream fichier(nomFichier);
		if (!fichier.is_open()) {
			cerr << "Erreur lors de l'ouverture du fichier " << nomFichier << endl;
			return maitres;
		}

		string ligne;
		getline(fichier, ligne);

		while (getline(fichier, ligne)) {
			stringstream ss(ligne);
			string champ;
			vector<string> champs;

			while (getline(ss, champ, ',')) {
				champs.push_back(champ);
			}

			if (champs.size() >= 1) {
				string nomMaitre = champs[0];
				vector<Pokemon*> equipe;
				for (size_t i = 3; i < champs.size(); ++i) {
					if (!champs[i].empty()) {
						auto it = pokedex.find(champs[i]);
						if (it != pokedex.end()) {
							equipe.push_back(it->second);
						}
					}
				}

				MaitrePokemon maitre(nomMaitre, equipe);
				maitres.push_back(maitre);
			}
		}

		fichier.close();
		return maitres;
	}

	static void enregistrerMaitreDansFichier(Entraineur& maitre, const string& nomFichier) {
		ofstream outFile(nomFichier, ios::app);
		if (!outFile) {
			cerr << "Erreur d'ouverture du fichier " << nomFichier << endl;
			return;
		}

		outFile << maitre.getNom() << ",";

		const auto& equipe = maitre.getEquipe();
		for (size_t i = 0; i < equipe.size(); ++i) {
			if (i < equipe.size()) {
				outFile << equipe[i]->getNom();
			}
			if (i != equipe.size() - 1) outFile << ",";
		}
		outFile << endl;
		outFile.close();
	}


	/// <summary>
	///	Le menu principal
	/// </summary>
	void Menu() {
		int choix = 0;
		do {
			cout << endl;
			cout << "=== MENU PRINCIPAL ===" << endl;
			cout << "1) Gerer mon equipe" << endl;
			cout << "2) Combattre" << endl;
			cout << "3) Statistiques" << endl;
			cout << "4) Interagir" << endl;
			cout << "5) Quitter" << endl;

			bool entreeValide = false;

			while (!entreeValide) {
				try {
					string saisie;
					cin >> saisie;

					if (!all_of(saisie.begin(), saisie.end(), ::isdigit)) {
						throw invalid_argument("Choix invalide, la saisie doit etre un entier.");
					}
					choix = stoi(saisie);
					if (choix < 1 || choix > 5) {
						throw out_of_range("Choix invalide, la saisie doit etre dans les options proposees.");
					}
					entreeValide = true;
				}
				catch (const exception& e) {
					cout << e.what() << " Veuillez reessayer." << endl;
				}
			}

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
				Interagir();
				break;
			case 5:
				cout << "Au revoir !" << endl;
				break;
			}

		} while (choix != 5);
	}
	
	/// <summary>
	/// L'option gerer mon équipe du menu principal
	/// </summary>
	void GererEquipe() {
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		if (!joueur) {
			cout << "Aucun joueur actif ! Veuillez démarrer un jeu." << endl;
			return;
		}
		cout << "=== GERER MON EQUIPE ===" << endl;
		cout << "1) Afficher mes pokemons" << endl;
		cout << "2) Soigner l'equipe" << endl;
		cout << "3) Changer l'ordre des pokemons" << endl;
		cout << "4) Retour" << endl;
		int choix;
		bool entreeValide = false;
		while (!entreeValide) {
			try {
				string saisie;
				cin >> saisie;

				if (!all_of(saisie.begin(), saisie.end(), ::isdigit)) {
					throw invalid_argument("Choix invalide, la saisie doit etre un entier.");
				}
				choix = stoi(saisie);
				if (choix < 1 || choix > 4) {
					throw out_of_range("Choix invalide, la saisie doit etre dans les options proposees.");
				}
				entreeValide = true;
			}
			catch (const exception& e) {
				cout << e.what() << " Veuillez reessayer." << endl;
			}
		}

		switch (choix) {
		case 1:
			joueur->afficherEquipe();
			break;
		case 2:
			joueur->soignerPokemon();
			break;
		case 3:
			joueur->changerOrdre();
			updateJoueurDansFichier(*joueur, fichierJoueurs);
			break;
		case 4:
			break;
		}
	}

	/// <summary>
	/// L'option combattre du menu principal
	/// </summary>
	void Combattre() {
		cout << "=== COMBATTRE ===" << endl;
		cout << "1) Affronter un joueur" << endl;
		cout << "2) Defi de gymnase" << endl;
		cout << "3) Maitre Pokemon (debloque apres 3 badges)" << endl;
		cout << "4) Retour" << endl;
		int choix;
		bool entreeValide = false;

		while (!entreeValide) {
			try {
				string saisie;
				cin >> saisie;

				if (!all_of(saisie.begin(), saisie.end(), ::isdigit)) {
					throw invalid_argument("Choix invalide, la saisie doit etre un entier.");
				}
				choix = stoi(saisie);
				if (choix < 1 || choix > 4) {
					throw out_of_range("Choix invalide, la saisie doit etre dans les options proposees.");
				}
				entreeValide = true;
			}
			catch (const exception& e) {
				cout << e.what() << " Veuillez reessayer." << endl;
			}
		}

			switch (choix) {
			case 1:
				AffronterJoueur();
				break;
			case 2:
				AffronterGymnase();
				break;
			case 3:
				AffronterMaitrePokemon();
				break;
			case 4:
				break;
			}
	}

	/// <summary>
	/// Le combat entre le joueur et un autre joueur
	/// </summary>
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
			cin.ignore();
			getline(cin, nomAdversaire);

			for (const string& nom : nomsJoueurs) {
				if (nom == nomAdversaire) {
					joueurTrouve = true;
					break;
				}
			}

			if (!joueurTrouve) {
				cout << "Joueur non trouve. Veuillez reessayer." << endl;
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

		bool tourDuJoueur = true;
		int round = 1;

		//boucle de combat : on continue jusqu'à qu'un joueur a mit 3 des pokémons de son adversaire KO
		while (joueur->getPokemonActif() != nullptr && adversaire.getPokemonActif() != nullptr) {
			cout << "\n===== ROUND " << round << " =====" << endl;
			Pokemon* p1 = joueur->getPokemonActif();
			Pokemon* p2 = adversaire.getPokemonActif();

			if (p1 == nullptr || p2 == nullptr) {
				cout << "Un des joueurs n'a plus de Pokemon utilisables." << endl;
				break;
			}

			cout << joueur->getNom() << " : ";
			cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

			cout << adversaire.getNom() << " : ";
			cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;

			cout << endl;

			if (tourDuJoueur) {
				//joueur attaque
				cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
				p1->attaquer(*p2);
				p2->recevoirDegats(p1->calculerDegats(*p2));
				this_thread::sleep_for(chrono::seconds(3));

				if (p2->estKo()) {
					cout << p2->getNom() << " est KO !" << endl;
					scoreJoueur++;
				}

				//adversaire attaque (si son Pokémon n’est pas KO)
				if (!p2->estKo()) {
					cout << endl;
					cout << joueur->getNom() << " : ";
					cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

					cout << adversaire.getNom() << " : ";
					cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
					cout << endl;

					cout << adversaire.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
					p2->attaquer(*p1);
					p1->recevoirDegats(p2->calculerDegats(*p1));
					this_thread::sleep_for(chrono::seconds(3));

					if (p1->estKo()) {
						cout << p1->getNom() << " est KO !" << endl;
						scoreAdversaire++;
					}
				}
			}
			else {
				//adversaire commence
				cout << adversaire.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
				p2->attaquer(*p1);
				p1->recevoirDegats(p2->calculerDegats(*p1));
				this_thread::sleep_for(chrono::seconds(3));

				if (p1->estKo()) {
					cout << p1->getNom() << " est KO !" << endl;
					scoreAdversaire++;
				}
				//joueur attaque (si son Pokémon n’est pas KO)
				if (!p1->estKo()) {
					cout << endl;
					cout << joueur->getNom() << " : ";
					cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

					cout << adversaire.getNom() << " : ";
					cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
					cout << endl;

					cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
					p1->attaquer(*p2);
					p2->recevoirDegats(p1->calculerDegats(*p2));
					this_thread::sleep_for(chrono::seconds(3));

					if (p2->estKo()) {
						cout << p2->getNom() << " est KO !" << endl;
						scoreJoueur++;
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
		if (adversaire.getPokemonActif() == nullptr) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur++
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire << endl;

			cout << joueurActif->getNom() << " a gagne le combat !" << endl;

			cout << joueur->getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << adversaire.getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			joueur->ajouterCombatGagne();
			adversaire.ajouterCombatPerdu();

			updateJoueurDansFichier(*joueur, fichierJoueurs);
			updateJoueurDansFichier(adversaire, fichierJoueurs);

			//comptabiliser la victoire
			joueurActif->ajouterAdversaireVaincu(adversaire.getNom());
			joueur->sauvegarderAdversaireVaincu(joueur->getNom(), adversaire.getNom(), listes_battus);
		}
		else if (joueur->getPokemonActif()==nullptr) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire++ << endl;
			cout << adversaire.getNom() << " a gagne le combat !" << endl;

			cout << adversaire.getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << joueur->getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			adversaire.ajouterCombatGagne();
			joueur->ajouterCombatPerdu();

			updateJoueurDansFichier(*joueur, fichierJoueurs);
			updateJoueurDansFichier(adversaire, fichierJoueurs);

			//comptabiliser la victoire
			adversaire.ajouterAdversaireVaincu(joueur->getNom());
			adversaire.sauvegarderAdversaireVaincu(adversaire.getNom(),joueur->getNom(), listes_battus);
		}
		else {
			cout << "Le combat est terminé." << endl;
		}

	}

	/// <summary>
	/// Le combat entre le joueur et un leader de gym
	/// </summary>
	void AffronterGymnase() {
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		vector<pair<string, string>> leaders;

		ifstream inFile(fichierLeaders);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << fichierLeaders << endl;
			return;
		}

		string ligne;
		getline(inFile, ligne); 

		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string nom, badge, champ2;
			getline(ss, nom, ',');
			getline(ss, champ2, ',');
			getline(ss, badge, ','); 

			if (!nom.empty() && !badge.empty()) {
				leaders.push_back({ nom, badge });
			}
		}

		inFile.close();
		cout << "=== Leaders disponibles ===" << endl;
		for (const auto& leader : leaders) {
			cout << "- " << leader.first << " : " << leader.second << endl;
		}
		string nomLeader;
		bool leaderTrouve = false;
		do {
			cout << "Quel leader voulez-vous affronter ? ";
			cin.ignore();
			getline(cin, nomLeader);

			for (const auto& leader : leaders) {
				if (leader.first == nomLeader) {
					leaderTrouve = true;
					break;
				}
			}

			if (!leaderTrouve) {
				cout << "Leader non trouve. Veuillez reessayer." << endl;
			}
		} while (!leaderTrouve);

		cout << "Vous allez affronter " << nomLeader << " !" << endl;
		LeaderGym leader;
		vector<LeaderGym> tousLesLeaders = chargerLeaderGymdepuisFichier(fichierLeaders, pokedex);
		for (auto l : tousLesLeaders) {
			if (l.getNom() == nomLeader) {
				leader = l;
				break;
			}
		}
		int scoreJoueur = 0;
		int scoreLeader = 0;
		bool tourDuJoueur = true;
		int round = 1;

		//boucle de combat : on continue jusqu'à qu'un joueur a mit 3 des pokémons de son adversaire KO
		while (joueur->getPokemonActif() != nullptr && leader.getPokemonActif() != nullptr) {
			cout << "\n===== ROUND " << round << " =====" << endl;
			Pokemon* p1 = joueur->getPokemonActif();
			Pokemon* p2 = leader.getPokemonActif();

			if (p1 == nullptr || p2 == nullptr) {
				cout << "Un des joueurs n'a plus de Pokemon utilisables." << endl;
				break;
			}

			cout << joueur->getNom() << " : ";
			cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

			cout << leader.getNom() << " : ";
			cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
			cout << endl;

			if (tourDuJoueur) {
				//joueur attaque
				cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
				p1->attaquer(*p2);
				p2->recevoirDegats(p1->calculerDegats(*p2));
				this_thread::sleep_for(chrono::seconds(3));

				if (p2->estKo()) {
					cout << p2->getNom() << " est KO !" << endl;
					scoreJoueur++;
				}

				//adversaire attaque (si son Pokémon n’est pas KO)
				if (!p2->estKo()) {
					cout << endl;
					cout << joueur->getNom() << " : ";
					cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;
					cout << leader.getNom() << " : ";
					cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
					cout << endl;

					cout << leader.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
					p2->attaquer(*p1);
					p1->recevoirDegats(p2->calculerDegats(*p1));
					this_thread::sleep_for(chrono::seconds(3));

					if (p1->estKo()) {
						cout << p1->getNom() << " est KO !" << endl;
						scoreLeader++;
					}
				}
			}
			else {
				//adversaire commence
				cout << leader.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
				p2->attaquer(*p1);
				p1->recevoirDegats(p2->calculerDegats(*p1));
				this_thread::sleep_for(chrono::seconds(3));

				if (p1->estKo()) {
					cout << p1->getNom() << " est KO !" << endl;
					scoreLeader++;
				}
				//joueur attaque (si son Pokémon n’est pas KO)
				if (!p1->estKo()) {
					cout << endl;
					cout << joueur->getNom() << " : ";
					cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;
					cout << leader.getNom() << " : ";
					cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
					cout << endl;

					cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
					p1->attaquer(*p2);
					p2->recevoirDegats(p1->calculerDegats(*p2));
					this_thread::sleep_for(chrono::seconds(3));

					if (p2->estKo()) {
						cout << p2->getNom() << " est KO !" << endl;
						scoreJoueur++;
					}
				}
			}
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << leader.getNom() << ": " << scoreLeader << endl;
			PauseConsole();
			round++;
			tourDuJoueur = !tourDuJoueur;
		}
		cout << endl;
		cout << "=== FIN DU COMBAT ===" << endl;
		PauseConsole();

		//fin du combat : on affiche les scores et qui a gagné
		if (leader.getPokemonActif() == nullptr) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur++
				<< " | " << leader.getNom() << ": " << scoreLeader << endl;
			cout << joueur->getNom() << " a gagne le combat !" << endl;
			cout << joueur->getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << leader.getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			//promouvoir le joueur en leader gym
			promouvoirEnLeaderGym(*joueur, leader);

			//ajouter le badge au joueur
			joueur->ajouterCombatGagne();
			cout << "Felecitations ! Vous avez gagne le badge " << leader.getBadgeGym() << " !" << endl;
			joueur->ajouterBadge(leader.getBadgeGym());
			updateJoueurDansFichier(*joueur, fichierJoueurs);

			//comptabiliser la victoire
			joueurActif->ajouterAdversaireVaincu(leader.getNom());
			joueur->sauvegarderAdversaireVaincu(joueur->getNom(), leader.getNom(), listes_battus);
		}
		else {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << leader.getNom() << ": " << scoreLeader++ << endl;

			cout << leader.getNom() << " a gagne le combat !" << endl;
			cout << leader.getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << joueur->getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			joueur->ajouterCombatPerdu();
			updateJoueurDansFichier(*joueur, fichierJoueurs);
		}
		

	}

	/// <summary>
	/// Le combat entre le joueur et le Maitre Pokemon
	/// </summary>
	void AffronterMaitrePokemon() {
		if (!joueurActif) {
			cout << "Aucun joueur actif !" << endl;
			return;
		}
		int bagesRequis = 3;
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		if (joueur->getBadges().size() < bagesRequis) {
			cout << "Vous devez avoir au moins " << bagesRequis << " badges pour affronter le Maitre Pokemon." << endl;
			return;
		}

		vector<MaitrePokemon> maitres = chargerMaitresDepuisFichier(fichierMaitres);
		if (maitres.empty()) {
			cout << "Aucun Maitre Pokemon disponible." << endl;
			return;
		}
		
		//on choisi un maitre pokemon au hasard
		srand(time(0));
		int index = rand() % maitres.size();

		MaitrePokemon adversaire = maitres[index];
		cout << "Vous allez affronter " << adversaire.getNom() << " !" << endl;
		int scoreJoueur = 0;
		int scoreAdversaire = 0;

		bool tourDuJoueur = true;
		int round = 1;

		//boucle de combat : on continue jusqu'à qu'un joueur a mit 3 des pokémons de son adversaire KO
		while (joueur->getPokemonActif() != nullptr && adversaire.getPokemonActif() != nullptr ) {
			cout << "\n===== ROUND " << round << " =====" << endl;
			Pokemon* p1 = joueur->getPokemonActif();
			Pokemon* p2 = adversaire.getPokemonActif();

			if (p1 == nullptr || p2 == nullptr) {
				cout << "Un des joueurs n'a plus de Pokemon utilisables." << endl;
				break;
			}

			cout << joueur->getNom() << " : ";
			cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

			cout << adversaire.getNom() << " : ";
			cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
			cout << endl;

			if (tourDuJoueur) {
				//joueur attaque
				cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
				p1->attaquer(*p2);
				p2->recevoirDegats(p1->calculerDegats(*p2));
				this_thread::sleep_for(chrono::seconds(3));

				if (p2->estKo()) {
					cout << p2->getNom() << " est KO !" << endl;
					scoreJoueur++;
				}

				//adversaire attaque (si son Pokémon n’est pas KO)
				if (!p2->estKo()) {
					cout << endl;
					cout << joueur->getNom() << " : ";
					cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

					cout << adversaire.getNom() << " : ";
					cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
					cout << endl;

					cout << adversaire.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
					p2->attaquer(*p1);  
					int degats = p2->calculerDegats(*p1);
					double bonus = adversaire.appliquerBonus(degats);
					int degatsFinal = static_cast<int>(bonus);

					if (degatsFinal > degats) {
						cout << "(Bonus maitre !) L'attaque passe de " << degats
							<< " a " << degatsFinal << " dégats !" << endl;
					}

					p1->recevoirDegats(degatsFinal);

					this_thread::sleep_for(chrono::seconds(3));


					if (p1->estKo()) {
						cout << p1->getNom() << " est KO !" << endl;
						scoreAdversaire++;
					}
				}
			}
			else {
				//adversaire commence
				cout << adversaire.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
				p2->attaquer(*p1);
				int degats = p2->calculerDegats(*p1);
				double bonus = adversaire.appliquerBonus(degats);
				int degatsFinal = static_cast<int>(bonus);

				if (degatsFinal > degats) {
					cout << "(Bonus maitre !) L'attaque passe de " << degats
						<< " a " << degatsFinal << " degats !" << endl;
				}

				p1->recevoirDegats(degatsFinal);

				this_thread::sleep_for(chrono::seconds(3));

				if (p1->estKo()) {
					cout << p1->getNom() << " est KO !" << endl;
					scoreAdversaire++;
				}

				//joueur attaque (si son Pokémon n’est pas KO)
				if (!p1->estKo()) {
					cout << endl;
					cout << joueur->getNom() << " : ";
					cout << p1->getNom() << " (" << p1->getHp() << " HP restants)" << endl;

					cout << adversaire.getNom() << " : ";
					cout << p2->getNom() << " (" << p2->getHp() << " HP restants)" << endl;
					cout << endl;

					cout << joueur->getNom() << " attaque avec " << p1->getNom() << " !" << endl;
					p1->attaquer(*p2);
					p2->recevoirDegats(p1->calculerDegats(*p2));
					this_thread::sleep_for(chrono::seconds(3));

					if (p2->estKo()) {
						cout << p2->getNom() << " est KO !" << endl;
						scoreJoueur++;
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
		if (adversaire.getPokemonActif() == nullptr) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire << endl;

			cout << joueurActif->getNom() << " a gagne le combat !" << endl;

			cout << joueur->getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << adversaire.getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			joueur->ajouterCombatGagne();
			updateJoueurDansFichier(*joueur, fichierJoueurs);

			//comptabiliser la victoire
			joueurActif->ajouterAdversaireVaincu(adversaire.getNom());
			joueur->sauvegarderAdversaireVaincu(joueur->getNom(), adversaire.getNom(), listes_battus);
			enregistrerMaitreDansFichier(*joueur, fichierMaitres); //le joueur devient maitre pokemon
		}
		else if (joueur->getPokemonActif() == nullptr) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << adversaire.getNom() << ": " << scoreAdversaire << endl;
			cout << adversaire.getNom() << " a gagne le combat !" << endl;

			cout << adversaire.getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << joueur->getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;

			joueur->ajouterCombatPerdu();
			updateJoueurDansFichier(*joueur, fichierJoueurs);
		}
		else {
			cout << "Le combat est terminé." << endl;
		}


	}

	/// <summary>
	/// Afficher les statistiques du joueur actif
	/// </summary>
	void Statistiques() {
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		if (joueur) {
			joueur->afficherStatistiques();
		}
		else {
			cout << "Aucun joueur actif !" << endl;
		}
	}

	/// <summary>
	/// L'option pour interagir avec les pokémons ou un adversaire vaincu
	/// </summary>
	void Interagir() {
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		cout << "=== INTERAGIR ===" << endl;
		cout << "1) Avec mes Pokemons" << endl;
		cout << "2) Avec un adversaire vaincu" << endl;

		int choix;
		bool entreeValide = false;

		while (!entreeValide) {
			try {
				string saisie;
				cin >> saisie;

				if (!all_of(saisie.begin(), saisie.end(), ::isdigit)) {
					throw invalid_argument("Choix invalide, la saisie doit etre un entier.");
				}
				choix = stoi(saisie);
				if (choix < 1 || choix > 2) {
					throw out_of_range("Choix invalide, la saisie doit etre dans les options proposees.");
				}
				entreeValide = true;
			}
			catch (const exception& e) {
				cout << e.what() << " Veuillez reessayer." << endl;
			}
		}

		switch (choix) {
		case 1:
			joueur->interagirPokemon();
			break;
		case 2:
			joueur->interagirAdversaire();
			break;
		}


	}

	/// <summary>
	/// Methode pour mettre en pause la console
	/// </summary>
	void PauseConsole() {
		cout << "Appuyez sur une touche pour continuer..." << endl;
		cin.ignore();
		cin.get();
	}

};

