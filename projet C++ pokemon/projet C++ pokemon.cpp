#include <iostream>
#include "Interface.h"
#include "Pokemon.h"
#include "PokemonFeu.h"
#include "PokemonEau.h"
#include "PokemonElectrik.h"

// Charger les Pokémon depuis un fichier CSV
map<string, Pokemon*> chargerPokemonDepuisFichier(const string& fichierNom) {
	map<string, Pokemon*> pokedex;
	ifstream fichier(fichierNom);
	if (!fichier) {
		cerr << "Erreur lors de l'ouverture du fichier." << endl;
		return pokedex;
	}
	string ligne;
	getline(fichier, ligne); // Lire la première ligne (en-tête)
	while (getline(fichier, ligne)) {
		stringstream ss(ligne);
		string nom, type1, type2, hpStr, nomAttaque, degatStr;
		getline(ss, nom, ',');
		getline(ss, type1, ',');
		getline(ss, type2, ',');
		getline(ss, hpStr, ',');
		getline(ss, nomAttaque, ',');
		getline(ss, degatStr, ',');
		int hp = stoi(hpStr);
		int degat = stoi(degatStr);
        

        Pokemon* poke = nullptr;
        if (type1 == "Feu") {
			poke = new PokemonFeu(nom, hp, nomAttaque, degat, "Eau");
		}
        else if (type1 == "Eau") {
			poke = new PokemonEau(nom, hp, nomAttaque, degat, "Electrik");
        }
		else if (type1 == "Electrik") {
			poke = new PokemonElectrik(nom, hp, nomAttaque, degat, "Sol");
        }
        pokedex[nom] = poke;
	}
	return pokedex;
}

//afficher le pokedex (le résultat est long, c'est juste pour tester)
void afficherPokedex(const map<string, Pokemon*>& pokedex) {
    cout << "=== POKEDEX ===" << endl;
    for (const auto& pair : pokedex) {
        if (!pair.second) {
            //le pokemon n'est pas chargé
            continue;
        }
        const Pokemon& poke = *pair.second;
        cout << "Nom: " << poke.getNom() << ", Type1: " << poke.getType1() << ", Type2: " << poke.getType2() << ", HP: " << poke.getHp() << endl;
        cout << "Attaques: ";
        for (const auto& attaque : poke.getAttaques()) {
            cout << attaque.first << " (Dégâts: " << attaque.second << "), ";
        }
        cout << endl;

    }
}


int main()
{
	string pokemonFichier = "pokemon.csv";
	string nomFichier = "joueur.csv";
	//ouvrir le fichier en mode lecture et écriture
	fstream myfile(nomFichier, ios::in | ios::out | ios::app);
	if (!myfile.is_open())
	{
		cout << "Error in opening the file" << endl;
		return 1;
	}
	map<string, Pokemon*> pokedex = chargerPokemonDepuisFichier(pokemonFichier);
	Interface interface(nomFichier, pokemonFichier, pokedex);

    int choix = 0;
    while (true) {
        cout << "=== MENU ===" << endl;
        cout << "1) Ajouter un joueur" << endl;
        cout << "2) Charger un joueur" << endl;
        cout << "3) Gerer mon equipe" << endl;
        cout << "4) Combattre" << endl;
        cout << "5) Mes statistiques" << endl;
        cout << "6) Sauvegarder et quitter" << endl;
        cout << "Votre choix : ";
        cin >> choix;

        while (choix < 1 || choix > 6) {
            cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 6." << endl;
            cin >> choix;
        }

        switch (choix) {
        case 1:
            interface.joueurActif = make_unique<Joueur>(
                Interface::creerNouveauJoueur(pokedex, nomFichier)
            );
            break;
        case 2: {
            vector<Joueur> joueurs = interface.chargerJoueursDepuisFichier(nomFichier, pokedex);
            if (joueurs.empty()) {
                cout << "Aucun joueur à charger." << endl;
            }
            else {
                cout << "Sélectionner le joueur à charger : " << endl;
                for (int i = 0; i < joueurs.size(); ++i) {
                    cout << i + 1 << ") " << joueurs[i].getNom() << endl;
                }
                int joueurChoisi;
                cin >> joueurChoisi;
                if (joueurChoisi >= 1 && joueurChoisi <= joueurs.size()) {
                    interface.joueurActif = make_unique<Joueur>(joueurs[joueurChoisi - 1]);
                    cout << "Joueur " << interface.joueurActif->getNom() << " chargé avec succès!" << endl;
                }
                else {
                    cout << "Choix invalide." << endl;
                }
            }
            break;
        }
        case 3:
            if (interface.joueurActif) {
               
            }
            else {
                cout << "Aucun joueur actif. Veuillez en charger ou en créer un." << endl;
            }
            break;
        case 4:
            interface.Combattre(); // Ajoute la vérification de joueur actif si nécessaire
            break;
        case 5:
            interface.Statistiques();
            break;
        case 6:
            cout << "Sauvegarde terminée. À bientôt !" << endl;
            return 0;
        }

        cout << "\n--- Retour au menu principal ---\n" << endl;
    }

	
}

