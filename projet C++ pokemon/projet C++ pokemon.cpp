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

//afficher le pokedex
void afficherPokedex(const map<string, Pokemon>& pokedex) {
	cout << "=== POKEDEX ===" << endl;
	for (const auto& pair : pokedex) {
		const Pokemon& poke = pair.second;
		cout << "Nom: " << poke.getNom() << ", Type1: " << poke.getType1() << ", Type2: " << poke.getType2() << ", HP: " << poke.getHp() << endl;
	}
}


int main()
{
	map<string, Pokemon> pokedex = chargerPokemonDepuisFichier("pokemon.csv");
	afficherPokedex(pokedex);
}

