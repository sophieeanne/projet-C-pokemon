#pragma once
#include "Pokemon.h"

class PokemonFeu : public Pokemon
{
public:
	//Constructeur
	PokemonFeu(string Nom, int Hp, string nomAttaque, int Degat, string Faiblesse)
		: Pokemon(Nom,"Feu", "", Hp, nomAttaque, Degat) {
	}

	//Méthodes virtuelles pures
	int calculerDegats(Pokemon& cible) override {
		//Les attaques du type PokemonFeu sont pas efficaces contre le type PokemonEau (les dégats sont multipliés par 0.5)
		float multiplicateur = 1.0f; //On suppose que le multiplicateur est neutre
		if (type1 == "Eau" || type2 == "Eau") {
			multiplicateur = 0.5f;
		}

		return static_cast<int>(attaques.begin()->second * multiplicateur);
	}

	void attaquer(Pokemon& cible) override {
		int degats = calculerDegats(cible);
		cout << nom << " attaque " << cible.getNom() << " avec ";

		//Afficher l'attaque (nom et puissance)
		auto it = attaques.begin();
		if (it != attaques.end()) {
			cout << it->first << " (puissance " << it->second << ")";
		}

		cout << " et inflige " << degats << "degats ! " << endl;
		cible.recevoirDegats(degats);
	}

	~PokemonFeu() override = default;

};

