#pragma once
#include "Pokemon.h"

class PokemonEau : public Pokemon
{
public:
	//Constructeur
	PokemonEau(string Nom, int Hp, string nomAttaque, int Degat, string Faiblesse)
		: Pokemon(Nom, "Eau" ,"", Hp, nomAttaque, Degat) {
		setFaiblesse(Faiblesse);
	}

	//Méthodes virtuelles pures
	int calculerDegats(Pokemon& cible) override {
		//Les attaques du type PokemonEau sont efficaces contre le type PokemonFeu (les dégats sont multipliés par 2)
		//Les attaques du type PokemonEau sont pas efficaces contre le type PokemonEau (les dégats sont multipliés par 0.5)

		float multiplicateur = 1.0f; //On suppose que le multiplicateur est neutre
		string typeAttaque = attaques.begin()->first;

		if (type1 == "Feu" || type2 == "Feu") {
			multiplicateur = 2.0f;
		}
		else if (type1 == "Eau" || type2 == "Eau") {
			multiplicateur = 0.5f;
		}
		//il y a un problème si un Pokemon est en même temps Eau et Feu... (possible ?)

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


	~PokemonEau() override = default;
	
};


