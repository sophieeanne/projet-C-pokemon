#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
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
	LeaderGym promouvoirEnLeaderGym(Joueur& joueur, const string& leaderBattu) {
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

			if (!champs.empty() && champs[0] == leaderBattu) {
				// Si on trouve le leader battu, on remplace la ligne par le nouveau leader
				string nouvelleLigne = leader.getNom() + "," + leader.getNomGym() + "," + leader.getBadgeGym();
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
			cerr << "Leader battu non trouvé : " << leaderBattu << endl;
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

		cout << joueur.getNom() << " a ete promu Leader de " << gym << " avec le badge " << badge << " !" << endl;
		return leader;
	}

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
		cin >> choix;
		do {
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
			default:
				cout << "Choix invalide. Veuillez reessayer." << endl;
				break;
			}
		} while (choix < 1 || choix > 4);
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
				AffronterGymnase();
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

	void AffronterGymnase() {
		vector<string> nomLeaders;
		Joueur* joueur = dynamic_cast<Joueur*>(joueurActif.get());
		ifstream inFile(fichierLeaders);
		if (!inFile) {
			cerr << "Erreur de lecture du fichier " << fichierLeaders << endl;
			return;
		}
		string ligne;
		getline(inFile, ligne);
		while (getline(inFile, ligne)) {
			stringstream ss(ligne);
			string champ;
			getline(ss, champ, ',');
			if (!champ.empty()) {
				nomLeaders.push_back(champ);
			}
		}
		inFile.close();
		cout << "=== Leaders disponibles ===" << endl;
		for (const string& nom : nomLeaders) {
			cout << "- " << nom << endl;
		}
		string nomLeader;
		bool leaderTrouve = false;
		do {
			cout << "Quel leader voulez-vous affronter ? ";
			cin >> nomLeader;

			for (const string& nom : nomLeaders) {
				if (nom == nomLeader) {
					leaderTrouve = true;
					break;
				}
			}

			if (!leaderTrouve) {
				cout << "Leader non trouvé. Veuillez réessayer." << endl;
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
		bool tourDuJoueur = rand() % 2 == 0;
		int round = 1;
		//boucle de combat : on continue jusqu'à qu'un joueur a mit 3 des pokémons de son adversaire KO
		while (scoreJoueur < 3 && scoreLeader < 3) {
			cout << "\n===== ROUND " << round << " =====" << endl;
			Pokemon* p1 = joueur->getPokemonActif();
			Pokemon* p2 = leader.getPokemonActif();

			if (p1 == nullptr || p2 == nullptr) {
				cout << "Un des joueurs n'a plus de Pokemon utilisables." << endl;
				break;
			}

			if (tourDuJoueur) {
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
					cout << leader.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
					p2->attaquer(*p1);
					p1->recevoirDegats(p2->calculerDegats(*p1));

					if (p1->estKo()) {
						cout << p1->getNom() << " est KO !" << endl;
						scoreLeader++;
						if (scoreLeader == 3) break;
					}
				}
			}
			else {
				//adversaire commence
				cout << leader.getNom() << " attaque avec " << p2->getNom() << " !" << endl;
				p2->attaquer(*p1);
				p1->recevoirDegats(p2->calculerDegats(*p1));

				if (p1->estKo()) {
					cout << p1->getNom() << " est KO !" << endl;
					scoreLeader++;
					if (scoreLeader == 3) break;
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
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << leader.getNom() << ": " << scoreLeader << endl;
			PauseConsole();
		}
		cout << endl;
		cout << "=== FIN DU COMBAT ===" << endl;
		PauseConsole();
		//fin du combat : on affiche les scores et qui a gagné
		if (scoreJoueur == 3) {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << leader.getNom() << ": " << scoreLeader << endl;
			cout << joueur->getNom() << " a gagne le combat !" << endl;
			cout << joueur->getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << leader.getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;
			promouvoirEnLeaderGym(*joueur, leader.getNom());
			joueur->ajouterCombatGagne();
		}
		else {
			cout << "Score - " << joueur->getNom() << ": " << scoreJoueur
				<< " | " << leader.getNom() << ": " << scoreLeader << endl;
			cout << leader.getNom() << " a gagne le combat !" << endl;
			cout << leader.getNom() << " : Encore une nouvelle victoire ! " << endl;
			cout << joueur->getNom() << " : Je vais m'entrainer pour la prochaine fois !" << endl;
			joueur->ajouterCombatPerdu();
			updateJoueurDansFichier(*joueur, fichierJoueurs);
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

