#include "Pokemon.h"
Pokemon::Pokemon(string Nom, vector<string> Type, int Hp, Attaque Attaque, int Degat, string Faiblesse) : nom(Nom), type(Type), hp(Hp), attaque(Attaque), degat(Degat), faiblesse(Faiblesse)  {}

void Animal::afficher() {
    cout << "Nom : " << nom << endl << "Type : " << type << endl << "Hp : " << hp << endl << "Attaque : " << attaque << endl << "Degat : " << degat << endl << "Faiblesse : " << faiblesse << endl;
}

void Pokemon::attaquer(Pokemon& cible) {
    
}

