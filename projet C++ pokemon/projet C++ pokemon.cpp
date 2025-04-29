#include <iostream>
#include "Interface.h"
#include "Pokemon.h"

// Charger les Pokémon depuis un fichier CSV
map<string, Pokemon> chargerPokemonDepuisFichier(const string& fichierNom) {
	map<string, Pokemon> pokedex;
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
		Pokemon poke(nom, type1, type2, hp, nomAttaque, degat);
		pokedex[nom] = poke;
	}
	return pokedex;
}

//afficher le pokedex (le résultat est long, c'est juste pour tester)
void afficherPokedex(const map<string, Pokemon>& pokedex) {
	cout << "=== POKEDEX ===" << endl;
	for (const auto& pair : pokedex) {
		const Pokemon& poke = pair.second;
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
	string nomFichier = "file1.txt";
	//ouvrir le fichier en mode lecture et écriture
	fstream myfile("file1.txt", ios::in | ios::out | ios::app);
	if (!myfile.is_open())
	{
		cout << "Error in opening the file" << endl;
		return 1;
	}
	map<string, Pokemon> pokedex = chargerPokemonDepuisFichier(pokemonFichier);
	Interface interface(nomFichier, pokemonFichier, pokedex);
	cout << "Bienvenue" << endl;
	cout << "1) Ajouter un joueur" << endl;
	cout << "2) Charger un joueur" << endl;
	cout << "2) Gerer mon equipe" << endl;
	cout << "3) Combattre" << endl;
	cout << "4) Mes statistiques" << endl;
	cout << "5) Interagir" << endl;
	cout << "6) Sauvegarder et quitter" << endl;
	int choix;
	cin >> choix;
	if (choix < 0 || choix>6) {
		do {
			cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 5" << endl;

		} while (choix < 0 || choix>6);
	}
	switch (choix) {
	case 0:
		interface.creerNouveauJoueur(pokedex, nomFichier);
		break;
	case 1:
		interface.chargerJoueursDepuisFichier(nomFichier, pokedex);
		break;
	case 2:

		break;
	case 3:
		interface.Statistiques();
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}
	
}

