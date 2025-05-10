#pragma once
#include "Pokemon.h"

class PokemonElectrik : public Pokemon
{
public:
	//Constructeur
	PokemonElectrik(string Nom, int Hp, string nomAttaque, int Degat, string Faiblesse)
		: Pokemon(Nom,"Electrik","", Hp, nomAttaque, Degat) {
		setFaiblesse(Faiblesse);
	}

	//Méthodes virtuelles pures
	int calculerDegats(Pokemon& cible) override {
		//Les attaques du type PokemonElectrik sont efficaces contre le type PokemonEau (les dégats sont multipliés par 2)
		//Les attaques du type PokemonElectrik sont pas efficaces contre le type PokemonElectrik (les dégats sont multipliés par 0.5)
		float multiplicateur = 1.0f; //On suppose que le multiplicateur est neutre

		if (cible.getType1() == "Eau" || cible.getType2() == "Eau") {
			multiplicateur = 2.0f;
		}
		else if (cible.getType2() == "Electrik") {
			multiplicateur = 0.5f;
		}
		//il y a un problème si un Pokemon est en même temps Eau et Electrik...

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

		cout << " et inflige " << degats << " degats ! " << endl;
		cible.recevoirDegats(degats);
	}

	void afficherMessageInteraction() override {
		cout << nom << ": \"Je suis en forme aujourd hui, comme une pile !\"" << endl;
	}

	~PokemonElectrik() override = default;
	
};

