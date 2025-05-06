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
        if (type1 == "Feu"||type2=="Feu") {
			poke = new PokemonFeu(nom, hp, nomAttaque, degat, "Eau");
		}
        else if (type1 == "Eau" || type2 == "Eau") {
			poke = new PokemonEau(nom, hp, nomAttaque, degat, "Electrik");
        }
		else if (type1 == "Electrik" || type2 == "Electrik") {
			poke = new PokemonElectrik(nom, hp, nomAttaque, degat, "Sol");
        }
		if (poke != nullptr) {
			pokedex[nom] = poke;
		}

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
            cout << attaque.first << " (Degats: " << attaque.second << "), ";
        }
        cout << endl;

    }
}


int main()
{
    //initialisaton de tous les fichiers et attributs nécessaires
	string pokemonFichier = "pokemon.csv";
	string nomFichier = "joueur.csv";
    map<string, Pokemon*> pokedex = chargerPokemonDepuisFichier(pokemonFichier);

	// ouvrir le fichier en lecture/écriture
	fstream myfile(nomFichier, ios::in | ios::out | ios::app);
	if (!myfile.is_open()) {
		cout << "Erreur dans l'ouverture du fichier" << endl;
		return 1;
	}

	//afficherPokedex(pokedex);


	unique_ptr<Entraineur> ja = nullptr;
	Interface interface(nomFichier, pokemonFichier, pokedex, nullptr);


	cout << "Bienvenue dans le jeu Pokemon !" << endl;
	cout << "1) Creer un nouvel utilisateur" << endl;
	cout << "2) Charger un utilisateur existant" << endl;

	int choix;
	cin >> choix;

	if (choix == 1) {
		ja = make_unique<Joueur>(Interface::creerNouveauJoueur(pokedex, nomFichier));
	}
	else if (choix == 2) {

		ja = Interface::choisirJoueurActif(nomFichier, pokedex); 
		if (!ja) {
			cout << "Le joueur n'existe pas !" << endl;
			return 1;
		}
	}
	interface.setJoueurActif(move(ja));
	//interface.Menu();
	//interface.Combattre();

	


	





	
}

