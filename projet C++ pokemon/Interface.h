#pragma once
using namespace std;
#include <string>
#include <iostream>
class Interface
{
public :
	Interface(){}
	void SetUp() {
		cout << "=== BIENVENUE DANS POKEMON ===" << endl;
		cout << "1) Nouvelle partie" << endl;
		cout << "2) Charger une partie" << endl;
		cout << "3) Quitter" << endl;
		int choix;
		cin >> choix;
		if (choix < 1 || choix>3) {
			do {
				cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 3" << endl;

			} while (choix < 1 || choix>3);
		}
		switch (choix) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
	}
	//Créer un nouveau joueur
	void Bienvenue() {
		cout << "Bienvenue *nom du joueur il faudra remplacer par le nom quand y'aura la classe entraineur*" << endl;
		cout << "1) Gerer mon equipe" << endl;
		cout << "2) Combattre" << endl;
		cout << "3) Mes statistiques" << endl;
		cout << "4) Interagir" << endl;
		cout << "5) Sauvegarder et quitter" << endl;
		int choix;
		cin >> choix;
		if (choix < 1 || choix>5) {
			do {
				cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 5" << endl;

			} while (choix < 1 || choix>5);
		}
		switch (choix) {
		case 1:
			GererEquipe();
			break;
		case 2:
			Combattre();
			break;
		case 3 :
			Statistiques();
			break;
		case 4 :
			break;
		case 5 :
			break;
		default :
			break;
		}
	}

	void GererEquipe() {
		cout << "=== GERER MON EQUIPE ===" << endl;
		cout << "1) Afficher mes pokemons" << endl;
		cout << "2) Changer l'ordre" << endl;
		cout << "3) Soigner l'equipe" << endl;
		cout << "4) Retour" << endl;
		int choix;
		cin >> choix;
		if (choix < 1 || choix>5) {
			do {
				cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 4" << endl;

			} while (choix < 1 || choix>5);
		}
		switch (choix) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			Bienvenue();
			break;
		default:
			break;
		}
	}

	void Combattre() {
		cout << "=== COMBATTRE ===" << endl;
		cout << "1) Affronter un joueur" << endl;
		cout << "2) Defi de gymnase" << endl;
		cout << "3) Maitre Pokemon (debloque apres 8 badges)" << endl;
		cout << "4) Retour" << endl;
		int choix;
		cin >> choix;
		if (choix < 1 || choix>5) {
			do {
				cout << "Choix invalide. Veuillez saisir un nombre entre 1 et 4" << endl;

			} while (choix < 1 || choix>5);
		}
		switch (choix) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			Bienvenue();
			break;
		default:
			break;
		}
	}	

	void Statistiques() {
		//modifier quand y'aura la classe Entraineur
	}
};

