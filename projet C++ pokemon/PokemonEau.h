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

	//M�thodes virtuelles pures

	/// <summary>
	/// M�thode qui calcul les degats re�us par une cible lorsqu'elle est attaqu�e par un PokemonEau
	/// </summary>
	/// <param name="cible">Pokemon cible</param>
	/// <returns>d�g�ts inflig�s � la cible</returns>
	int calculerDegats(Pokemon& cible) override {
		//Les attaques du type PokemonEau sont efficaces contre le type PokemonFeu (les d�gats sont multipli�s par 2)
		//Les attaques du type PokemonEau sont pas efficaces contre le type PokemonEau (les d�gats sont multipli�s par 0.5)

		float multiplicateur = 1.0f; //On suppose que le multiplicateur est neutre
		string typeAttaque = attaques.begin()->first;

		if (cible.getType1() == "Feu" || cible.getType2() == "Feu") {
			multiplicateur = 2.0f;
		}
		else if (cible.getType1() == "Eau" || cible.getType2() == "Eau") {
			multiplicateur = 0.5f;
		}

		return static_cast<int>(attaques.begin()->second * multiplicateur);
	}

	/// <summary>
	/// M�thode qui g�re une attaque en affichant l'attaque et en mettant � jour les points de vie de la cible apr�s l'attaque
	/// </summary>
	/// <param name="cible">Pok�mon cible</param>
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
	/// M�thode qui affiche la phrase d'int�raction
	/// </summary>
	void afficherMessageInteraction() override {
		cout << nom << ": \"C est l heure de la baignade ou du combat ?\"" << endl;
	}

	~PokemonEau() override = default;
	
};


