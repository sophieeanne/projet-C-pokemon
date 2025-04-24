#pragma once
#ifndef POKEMON
#define POKEMON

#include <iostream>
#include <string>
using namespace std;

class Pokemon {
protected:
    string nom;
    vector<string> type;
    int hp;
    Attaque attaque;
    int degat;
    string faiblesse;

public:
    Pokemon(string Nom, vector<string> Type, int Hp, Attaque Attaque, int Degat, string Faiblesse);
    void afficher();
};

#endif

class Pokemon
{
};
