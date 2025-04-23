#include "Pokemon.h"
Animal::Animal(string nom, int age) : nom(nom), age(age) {}

void Animal::afficher() {
    cout << "Nom : " << nom << endl << "Age : " << age << " ans" << endl;
}
