#pragma once
#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>
#include <string>
using namespace std;

class Animal {
protected:
    string nom;
    int age;

public:
    Animal(string nom, int age);
    void afficher();
};

#endif
