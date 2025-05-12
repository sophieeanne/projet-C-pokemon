#pragma once
#include "Pokemon.h"

class PokemonFeu : public Pokemon
{
public:
	//Constructeur
	PokemonFeu(string Nom, int Hp, string nomAttaque, int Degat, string Faiblesse)
		: Pokemon(Nom,"Feu", "", Hp, nomAttaque, Degat, Faiblesse) {
	}

	//Méthodes virtuelles pures

	/// <summary>
	/// Méthode qui calcul les degats reçus par une cible lorsqu'elle est attaquée par un PokemonFeu
	/// </summary>
	/// <param name="cible">Pokemon cible</param>
	/// <returns>dégâts infligés à la cible</returns>
	int calculerDegats(Pokemon& cible) override {
		//Les attaques du type PokemonFeu sont pas efficaces contre le type PokemonEau (les dégats sont multipliés par 0.5)
		float multiplicateur = 1.0f; //On suppose que le multiplicateur est neutre
		if (cible.getType1() == "Eau" || cible.getType2() == "Eau") {
			multiplicateur = 0.5f;
		}

		return static_cast<int>(attaques.begin()->second * multiplicateur);
	}

	/// <summary>
	/// Méthode qui gère une attaque en affichant l'attaque et en mettant à jour les points de vie de la cible après l'attaque
	/// </summary>
	/// <param name="cible">Pokémon cible</param>
	void attaquer(Pokemon& cible) override {
		int degats = calculerDegats(cible);
		cout << nom << " attaque " << cible.getNom() << " avec ";

		//Afficher l'attaque (nom et puissance)
		auto it = attaques.begin();
		if (it != attaques.end()) {
			cout << it->first << " (puissance " << it->second << ")";
		}

		cout << " et inflige " << degats << " degats ! " << endl;
		//cible.recevoirDegats(degats);
	}

	/// <summary>
	/// Méthode qui affiche la phrase d'intéraction
	/// </summary>
	void afficherMessageInteraction() override {
		cout << nom << ": \"Je ne suis pas suceptible... sauf quand il pleut.\"" << endl;
	}

	~PokemonFeu() override = default;

};

